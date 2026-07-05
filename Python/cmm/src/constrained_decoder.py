"""Grammar-constrained, schema-aware JSON generation.

Instead of hoping the model spontaneously produces valid JSON, every token
of the output is chosen from a small set of grammar-legal candidates:

* Literal parts of the JSON skeleton (keys, punctuation, quotes) are
  deterministic, so they are injected directly without ever calling the
  model.
* Parts that require an actual decision (which function to call, which
  value to put in each argument) are generated token-by-token with the
  model's logits masked down to only the tokens that keep the output both
  syntactically valid JSON *and* compliant with the function's schema.

This guarantees 100% valid, schema-compliant JSON while still letting the
LLM (not a heuristic) decide the function name and every argument value.
"""

from __future__ import annotations

import re
import sys
from dataclasses import dataclass, field
from typing import Callable, Protocol

import numpy as np

from src.tokenizer import TokenizerBackend

_MAX_VALUE_TOKENS = 64

_NUMBER_PREFIX_RE = re.compile(r"^-?(0|[1-9][0-9]*)?(\.[0-9]*)?$")
_NUMBER_COMPLETE_RE = re.compile(r"^-?(0|[1-9][0-9]*)(\.[0-9]+)?$")
_INTEGER_COMPLETE_RE = re.compile(r"^-?(0|[1-9][0-9]*)$")


def _is_complete_number(text: str) -> bool:
    return bool(_NUMBER_COMPLETE_RE.match(text))


def _is_complete_integer(text: str) -> bool:
    return bool(_INTEGER_COMPLETE_RE.match(text))


_MIN_REPEAT_UNIT = 3
_MAX_REPEAT_UNIT = 20
_REPEAT_COUNT = 3


def _find_repetition_trim_point(text: str) -> str | None:
    """Detect a small-model failure mode: the same chunk repeated verbatim.

    Constrained decoding only guarantees *syntactic* validity; it cannot stop
    the model from getting stuck looping the same phrase (e.g. repeating a
    regex fragment). This looks for a unit of 3-20 characters repeated 3
    times in a row at the end of the text and, if found, returns ``text``
    trimmed down to a single copy of that unit - so the value grammar can
    force a clean stop instead of running the loop out to the token budget
    (and keeping the redundant copies).

    Returns ``None`` if no such repetition is present.
    """
    for unit_len in range(_MIN_REPEAT_UNIT, _MAX_REPEAT_UNIT + 1):
        needed = unit_len * _REPEAT_COUNT
        if len(text) < needed:
            continue
        tail = text[-needed:]
        if tail != tail[:unit_len] * _REPEAT_COUNT:
            continue
        # The tail is periodic with period `unit_len`, but its phase may not
        # match where the repeating run actually starts (e.g. it could be a
        # rotation of the "natural" unit). Extend backward one character at
        # a time to find the true start of the run before trimming.
        run_start = len(text) - needed
        while run_start > 0 and text[run_start - 1] == text[run_start - 1 + unit_len]:
            run_start -= 1
        return text[: run_start + unit_len]
    return None


ContinuationFn = Callable[[str], dict[int, str]]
CompleteFn = Callable[[str], bool]
RepairFn = Callable[[str], str | None]


class LLMBackend(Protocol):
    """Structural type for the subset of ``Small_LLM_Model`` this module needs.

    Notably, this does *not* include ``encode``/``decode``: tokenizing text
    is handled entirely by our own :class:`~src.tokenizer.Tokenizer`, built
    only from ``get_path_to_vocab_file``/``get_path_to_merges_file``.
    """

    def get_logits_from_input_ids(self, input_ids: list[int]) -> list[float]:
        ...


class GenerationError(RuntimeError):
    """Raised when constrained decoding cannot make progress."""


def _tokens_that_are_prefix_of(text_to_id: dict[str, int], target: str) -> dict[int, str]:
    """Vocabulary tokens whose text is a non-empty prefix of ``target``.

    Only ``len(target)`` dictionary lookups are needed (instead of scanning
    the whole vocabulary), since a prefix of ``target`` can only be one of
    its ``len(target)`` leading substrings.
    """
    matches: dict[int, str] = {}
    for length in range(1, len(target) + 1):
        candidate = target[:length]
        token_id = text_to_id.get(candidate)
        if token_id is not None:
            matches[token_id] = candidate
    return matches


@dataclass(frozen=True)
class VocabIndex:
    """Precomputed, reusable views over the model vocabulary.

    Building these views once at startup (a single O(vocab size) pass)
    instead of re-scanning the whole vocabulary at every generation step
    keeps constrained decoding fast even though the SDK offers no KV-cache
    and vocabularies can hold well over 100k tokens.
    """

    id_to_text: dict[int, str]
    text_to_id: dict[str, int] = field(default_factory=dict)
    digit_token_ids: list[int] = field(default_factory=list)
    dot_token_id: int | None = None
    minus_token_id: int | None = None
    safe_string_token_ids: list[int] = field(default_factory=list)

    @classmethod
    def build(cls, id_to_text: dict[int, str]) -> "VocabIndex":
        digit_re = re.compile(r"^[0-9]+$")
        forbidden_in_strings = {chr(c) for c in range(0x20)} | {'"', "\\"}

        text_to_id: dict[str, int] = {}
        digit_ids: list[int] = []
        dot_id: int | None = None
        minus_id: int | None = None
        safe_string_ids: list[int] = []

        for token_id, text in id_to_text.items():
            if text == "":
                continue
            text_to_id.setdefault(text, token_id)
            if digit_re.match(text):
                digit_ids.append(token_id)
            elif text == "." and dot_id is None:
                dot_id = token_id
            elif text == "-" and minus_id is None:
                minus_id = token_id
            if not (set(text) & forbidden_in_strings):
                safe_string_ids.append(token_id)

        return cls(
            id_to_text=id_to_text,
            text_to_id=text_to_id,
            digit_token_ids=digit_ids,
            dot_token_id=dot_id,
            minus_token_id=minus_id,
            safe_string_token_ids=safe_string_ids,
        )


class ConstrainedJSONGenerator:
    """Drives token-by-token, grammar-constrained decoding for one prompt."""

    def __init__(
        self,
        llm: LLMBackend,
        tokenizer: TokenizerBackend,
        vocab: VocabIndex,
        trace: bool = False,
    ) -> None:
        self._llm = llm
        self._tokenizer = tokenizer
        self._vocab = vocab
        self._trace = trace

    def _encode(self, text: str) -> list[int]:
        if text == "":
            return []
        return self._tokenizer.encode(text)

    def _log(self, message: str) -> None:
        """Print a step of the generation process to stderr, if tracing."""
        if self._trace:
            print(f"[trace] {message}", file=sys.stderr)

    @staticmethod
    def _argmax_among(logits: list[float], candidate_ids: list[int]) -> int:
        """Return the id in ``candidate_ids`` with the highest logit score."""
        if not candidate_ids:
            raise GenerationError("No grammar-legal token is available at this generation step.")
        logits_array = np.asarray(logits, dtype=np.float64)
        candidates_array = np.asarray(candidate_ids, dtype=np.int64)
        best_local_index = int(np.argmax(logits_array[candidates_array]))
        return int(candidates_array[best_local_index])

    def force_literal(self, input_ids: list[int], literal: str) -> list[int]:
        """Append a deterministic (non-generated) piece of the JSON skeleton."""
        return input_ids + self._encode(literal)

    def _consume(
        self,
        input_ids: list[int],
        literal_after: str,
        continuation_candidates: ContinuationFn,
        is_complete: CompleteFn,
        repair: RepairFn | None = None,
    ) -> tuple[str, list[int]]:
        """Token-by-token loop shared by every value grammar (enum/number/string).

        At each step the legal next tokens are: (a) tokens that extend the
        value while keeping it a valid prefix, and, once the value already
        forms a complete valid value, (b) a single "stop" token: the first
        token of the literal that must follow. Because both options are
        scored through the very same argmax over logits, the *model*
        decides how long the value should be, instead of a hand-written
        heuristic deciding for it.

        ``repair``, when given, is checked after every accepted token once
        the value is already complete; if it returns a (trimmed) string,
        decoding stops immediately using that string as the final value,
        even though the grammar would otherwise still allow continuing.
        This is a safety net against a real small-model failure mode
        (getting stuck looping the same phrase) that constrained decoding by
        itself cannot prevent, since a repeated phrase is still
        syntactically valid. The returned token ids are re-derived from the
        repaired value (instead of the raw, looping generation) so later
        steps never see the discarded repetition in their context.
        """
        literal_ids = self._encode(literal_after)
        stop_id = literal_ids[0] if literal_ids else None
        start_ids = input_ids

        current = ""
        for _ in range(_MAX_VALUE_TOKENS):
            candidates = continuation_candidates(current)
            if stop_id is not None and is_complete(current) and stop_id not in candidates:
                candidates = dict(candidates)
                candidates[stop_id] = self._vocab.id_to_text.get(stop_id, "")

            if not candidates:
                if is_complete(current):
                    self._log(f"stop (grammar exhausted) -> {current!r}")
                    return current, input_ids + literal_ids
                raise GenerationError("The model produced no grammar-legal continuation for this value.")

            if len(candidates) == 1:
                chosen_id = next(iter(candidates))
                self._log(f"forced (single legal token) -> {candidates[chosen_id]!r}")
            else:
                logits = self._llm.get_logits_from_input_ids(input_ids)
                chosen_id = self._argmax_among(logits, list(candidates.keys()))
                self._log(f"model chose {candidates[chosen_id]!r} among {len(candidates)} candidates")

            if chosen_id == stop_id:
                self._log(f"stop (model-chosen) -> {current!r}")
                return current, input_ids + literal_ids

            current += candidates[chosen_id]
            input_ids = input_ids + [chosen_id]

            if repair is not None and is_complete(current):
                corrected = repair(current)
                if corrected is not None:
                    self._log(f"repaired degenerate repetition: {current!r} -> {corrected!r}")
                    return corrected, start_ids + self._encode(corrected) + literal_ids

        if is_complete(current):
            self._log(f"stop (token budget reached) -> {current!r}")
            return current, input_ids + literal_ids
        raise GenerationError(f"Exceeded the token budget ({_MAX_VALUE_TOKENS}) while generating a value.")

    def _choice_candidates(self, choices: list[str]) -> ContinuationFn:
        text_to_id = self._vocab.text_to_id

        def continuation(current: str) -> dict[int, str]:
            result: dict[int, str] = {}
            for choice in choices:
                if not choice.startswith(current):
                    continue
                suffix = choice[len(current):]
                if suffix:
                    result.update(_tokens_that_are_prefix_of(text_to_id, suffix))
            return result

        return continuation

    def _number_candidates(self, allow_decimal: bool) -> ContinuationFn:
        pool = self._vocab.digit_token_ids
        id_to_text = self._vocab.id_to_text
        dot_id = self._vocab.dot_token_id
        minus_id = self._vocab.minus_token_id

        def continuation(current: str) -> dict[int, str]:
            result: dict[int, str] = {}
            for token_id in pool:
                text = id_to_text[token_id]
                if _NUMBER_PREFIX_RE.match(current + text):
                    result[token_id] = text
            if allow_decimal and dot_id is not None and current not in ("", "-") and "." not in current:
                result[dot_id] = "."
            if minus_id is not None and current == "":
                result[minus_id] = "-"
            return result

        return continuation

    def _string_candidates(self) -> ContinuationFn:
        fixed = {token_id: self._vocab.id_to_text[token_id] for token_id in self._vocab.safe_string_token_ids}
        return lambda _current: fixed

    def generate_choice(self, input_ids: list[int], choices: list[str], literal_after: str) -> tuple[str, list[int]]:
        """Generate one of a fixed, known set of literal strings.

        Used both for picking the function name (enum of function names)
        and for boolean literals (``"true"`` / ``"false"``).
        """
        return self._consume(input_ids, literal_after, self._choice_candidates(choices), lambda c: c in choices)

    def generate_value(self, input_ids: list[int], schema_type: str, literal_after: str) -> tuple[object, list[int]]:
        """Generate a single argument value according to its schema type.

        Args:
            input_ids: The token ids generated so far.
            schema_type: One of ``"number"``, ``"integer"``, ``"string"``,
                ``"boolean"``.
            literal_after: The deterministic JSON text that must follow this
                value (e.g. ``', "b": '`` or ``'}'``).

        Returns:
            A tuple of the Python value (already cast to its expected type)
            and the updated token id sequence.

        Raises:
            GenerationError: If ``schema_type`` is not supported, or if the
                model cannot produce a grammar-legal value within budget.
        """
        if schema_type == "string":
            candidates_fn = self._string_candidates()
            text, input_ids = self._consume(
                input_ids, literal_after, candidates_fn, lambda _c: True, repair=_find_repetition_trim_point
            )
            return text, input_ids
        if schema_type == "boolean":
            text, input_ids = self.generate_choice(input_ids, ["true", "false"], literal_after)
            return text == "true", input_ids
        if schema_type == "number":
            candidates_fn = self._number_candidates(allow_decimal=True)
            text, input_ids = self._consume(input_ids, literal_after, candidates_fn, _is_complete_number)
            return float(text), input_ids
        if schema_type == "integer":
            candidates_fn = self._number_candidates(allow_decimal=False)
            text, input_ids = self._consume(input_ids, literal_after, candidates_fn, _is_complete_integer)
            return int(text), input_ids
        raise GenerationError(f"Unsupported parameter type: {schema_type!r}")
