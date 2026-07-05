"""Tests for the grammar-constrained JSON generation engine."""

import pytest

from src.constrained_decoder import ConstrainedJSONGenerator, GenerationError, VocabIndex, _find_repetition_trim_point
from tests.fakes import FakeLLM, build_fake_tokenizer, build_fake_vocab


def _generator() -> ConstrainedJSONGenerator:
    vocab = VocabIndex.build(build_fake_vocab())
    return ConstrainedJSONGenerator(FakeLLM(), build_fake_tokenizer(), vocab)


def test_find_repetition_trim_point_handles_phase_shifted_repeats() -> None:
    """The exact string produced by the real Qwen3-0.6B model on a hard
    regex-extraction prompt: 3 repeats of an 11-character unit, plus 2 extra
    characters from a 4th attempt. The trim point must land after a single,
    clean copy of the unit, even though the tail-based detection sees a
    phase-shifted rotation of it.
    """
    text = "([0-9]+) | ([0-9]+) | ([0-9]+) | (["

    trimmed = _find_repetition_trim_point(text)

    assert trimmed == "([0-9]+) | "


def test_find_repetition_trim_point_returns_none_without_repetition() -> None:
    assert _find_repetition_trim_point("hello world") is None


def test_vocab_index_categorizes_tokens() -> None:
    vocab = VocabIndex.build(build_fake_vocab())

    assert len(vocab.digit_token_ids) == 10
    assert vocab.dot_token_id is not None
    assert vocab.minus_token_id is not None
    assert vocab.id_to_text[vocab.dot_token_id] == "."
    assert vocab.id_to_text[vocab.minus_token_id] == "-"
    assert ord('"') not in vocab.safe_string_token_ids
    assert ord("\\") not in vocab.safe_string_token_ids
    assert ord("a") in vocab.safe_string_token_ids


def test_generate_value_number_is_never_empty() -> None:
    generator = _generator()

    value, _ = generator.generate_value([], "number", "}")

    assert isinstance(value, float)
    assert value >= 0


def test_generate_value_number_never_stops_before_first_digit() -> None:
    """Even if the backend strongly prefers the closing brace, at least one
    digit must be produced: an empty number is never grammar-legal, so the
    stop token cannot be a candidate until the value is already complete.
    """

    class AlwaysPreferBraceLLM:
        def get_logits_from_input_ids(self, input_ids: list[int]) -> list[float]:
            logits = [0.0] * 128
            logits[ord("}")] = 10_000.0
            return logits

    vocab = VocabIndex.build(build_fake_vocab())
    generator = ConstrainedJSONGenerator(AlwaysPreferBraceLLM(), build_fake_tokenizer(), vocab)

    value, _ = generator.generate_value([], "number", "}")

    assert isinstance(value, float)


def test_generate_value_integer_rejects_decimal_point() -> None:
    generator = _generator()

    value, _ = generator.generate_value([], "integer", "}")

    assert isinstance(value, int)


def test_generate_value_string_produces_valid_content() -> None:
    generator = _generator()

    value, _ = generator.generate_value([], "string", '"')

    assert isinstance(value, str)
    assert '"' not in value
    assert "\\" not in value


def test_generate_value_boolean_returns_python_bool() -> None:
    generator = _generator()

    value, _ = generator.generate_value([], "boolean", "}")

    assert isinstance(value, bool)


def test_generate_value_unsupported_type_raises() -> None:
    generator = _generator()

    with pytest.raises(GenerationError):
        generator.generate_value([], "array", "}")


def test_generate_value_string_trims_degenerate_repetition() -> None:
    """Reproduces a real failure observed with the actual Qwen3-0.6B model:
    for a hard prompt it looped a regex fragment ("([0-9]+) | " repeated)
    until the token budget was exhausted. A backend that always strongly
    prefers to keep cycling through the same 4-character pattern must be
    trimmed down to a single copy of that pattern, not just cut off
    wherever the repetition happened to be noticed.
    """

    class CyclicPatternLLM:
        def __init__(self, pattern: str) -> None:
            self._pattern = pattern
            self._step = 0

        def get_logits_from_input_ids(self, input_ids: list[int]) -> list[float]:
            favored = self._pattern[self._step % len(self._pattern)]
            self._step += 1
            logits = [0.0] * 128
            logits[ord(favored)] = 10_000.0
            return logits

    vocab = VocabIndex.build(build_fake_vocab())
    generator = ConstrainedJSONGenerator(CyclicPatternLLM("abcd"), build_fake_tokenizer(), vocab)

    value, _ = generator.generate_value([], "string", '"')

    assert value == "abcd"


def test_generate_choice_resolves_to_one_of_the_choices() -> None:
    generator = _generator()

    choice, _ = generator.generate_choice([], ["fn_add_numbers", "fn_greet"], '"}')

    assert choice in {"fn_add_numbers", "fn_greet"}


def test_force_literal_appends_encoded_tokens() -> None:
    generator = _generator()

    ids = generator.force_literal([1, 2, 3], "ab")

    assert ids == [1, 2, 3, ord("a"), ord("b")]


def test_trace_mode_prints_steps_to_stderr(capsys: pytest.CaptureFixture[str]) -> None:
    vocab = VocabIndex.build(build_fake_vocab())
    generator = ConstrainedJSONGenerator(FakeLLM(), build_fake_tokenizer(), vocab, trace=True)

    generator.generate_value([], "integer", "}")

    assert "[trace]" in capsys.readouterr().err
