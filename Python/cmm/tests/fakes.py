"""Deterministic fakes for the LLM backend and tokenizer used across the test suite.

They implement the same structural interfaces as ``Small_LLM_Model`` and
:class:`~src.tokenizer.Tokenizer` without downloading or running any real
model, so the constrained-decoding *grammar* can be tested in isolation from
the LLM's semantic quality (accuracy of the real Qwen3-0.6B model is
validated separately by actually running the pipeline, see the README's
"Testing strategy" section).
"""

from src.tokenizer import Tokenizer
from src.tokenizer_vocab import byte_to_unicode

_STOP_CHARS = {",", "}", '"'}
_VOCAB_SIZE = 128


def build_fake_vocab() -> dict[int, str]:
    """A one-character-per-token *decoded* vocabulary, for grammar checks."""
    return {code: chr(code) for code in range(_VOCAB_SIZE)}


def build_fake_tokenizer() -> Tokenizer:
    """A real :class:`Tokenizer`, backed by a one-byte-per-token raw vocabulary.

    Built the same way :func:`src.tokenizer_vocab.load_raw_vocab` would from
    a real ``vocab.json``, just restricted to the ASCII range and with no
    merge rules - so encoding stays one token per character, matching
    :func:`build_fake_vocab`'s id numbering exactly (both are keyed by the
    same underlying byte value).
    """
    raw_vocab = {byte_to_unicode()[byte]: byte for byte in range(_VOCAB_SIZE)}
    return Tokenizer(vocab=raw_vocab, merges={})


class FakeLLM:
    """Deterministic stand-in for ``Small_LLM_Model``.

    ``get_logits_from_input_ids`` prefers ending a value (comma/brace/quote)
    once a couple of tokens have been generated, and otherwise favors
    alphanumeric characters, giving short but non-trivial deterministic
    values for tests.
    """

    def __init__(self) -> None:
        self.calls = 0

    def get_logits_from_input_ids(self, input_ids: list[int]) -> list[float]:
        self.calls += 1
        prefer_stop = self.calls > 2
        logits = [0.0] * _VOCAB_SIZE
        for code in range(_VOCAB_SIZE):
            char = chr(code)
            if prefer_stop and char in _STOP_CHARS:
                logits[code] = 1000.0
            elif not prefer_stop and char.isalnum():
                logits[code] = 500.0 + code
            else:
                logits[code] = float(code)
        return logits
