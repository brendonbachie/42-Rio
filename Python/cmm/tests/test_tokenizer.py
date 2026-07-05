"""Tests for the from-scratch byte-level BPE tokenizer (src/tokenizer.py).

This tokenizer is what the bonus "recoded tokenizer" is about: the main
code path never calls the SDK's own ``encode``/``decode``, only
``get_logits_from_input_ids`` and the vocab/merges file paths. These tests
validate the encoder/decoder in isolation, without needing the real model.
"""

from src.tokenizer import Tokenizer, _split_words
from tests.fakes import build_fake_tokenizer

_ROUND_TRIP_SAMPLES = [
    "",
    "hello",
    "hello world",
    "What is the sum of 2 and 3?",
    "Reverse the string 'hello'",
    '{"name": "fn_add_numbers", "parameters": {"a": 2.0, "b": 3.0}}',
    "  two leading spaces",
    "line one\nline two",
    "trailing space ",
]


def test_encode_decode_round_trips_for_common_text() -> None:
    tokenizer = build_fake_tokenizer()

    for text in _ROUND_TRIP_SAMPLES:
        assert tokenizer.decode(tokenizer.encode(text)) == text


def test_merges_combine_symbols_into_fewer_tokens() -> None:
    """A dedicated merge rule must actually reduce the token count."""
    vocab = {"a": 0, "b": 1, "ab": 2}

    without_merges = Tokenizer(vocab, merges={})
    with_merges = Tokenizer(vocab, merges={("a", "b"): 0})

    assert without_merges.encode("ab") == [0, 1]
    assert with_merges.encode("ab") == [2]


def test_merges_apply_highest_priority_pair_first() -> None:
    """Lower rank (earlier in merges.txt) must win over a later-ranked pair."""
    vocab = {"a": 0, "b": 1, "c": 2, "bc": 3, "abc": 4}
    merges = {("a", "b"): 1, ("b", "c"): 0}  # "bc" has priority over "ab"

    tokenizer = Tokenizer(vocab, merges)

    assert tokenizer.encode("abc") == [0, 3]


def test_decode_skips_unknown_token_ids_gracefully() -> None:
    tokenizer = build_fake_tokenizer()

    assert tokenizer.decode([ord("h"), ord("i"), 999_999]) == "hi"


def test_encode_word_splitting_treats_letters_digits_and_punctuation_separately() -> None:
    tokenizer = build_fake_tokenizer()

    ids = tokenizer.encode("a1!")

    assert tokenizer.decode(ids) == "a1!"
    assert len(ids) == 3


def test_special_tokens_are_encoded_as_a_single_atomic_id() -> None:
    """Regression test: chat-template markers such as ``<|im_start|>`` must
    never be split into individual characters by the byte-level BPE
    algorithm, exactly like a real tokenizer's added/special tokens.

    This reproduces a real failure observed with the actual model: without
    this, `<|im_start|>`/`<|im_end|>` were tokenized character-by-character
    (they are *not* present in ``vocab.json`` at all - real tokenizers keep
    added/special tokens in a separate table, e.g. ``tokenizer.json``'s
    ``added_tokens``, with ids outside the base vocabulary), which confused
    the model badly enough to pick the wrong function and leak literal
    `<|im_end|>` fragments into a generated string.
    """
    vocab = {"a": 0, "b": 1}
    special_tokens = {"<|im_start|>": 100, "<|im_end|>": 101}

    tokenizer = Tokenizer(vocab, merges={}, special_tokens=special_tokens)

    assert tokenizer.encode("<|im_start|>ab<|im_end|>") == [100, 0, 1, 101]


def test_special_tokens_not_appearing_in_text_are_harmless() -> None:
    vocab = {"a": 0, "b": 1}
    special_tokens = {"<|im_start|>": 100}

    tokenizer = Tokenizer(vocab, merges={}, special_tokens=special_tokens)

    assert tokenizer.decode(tokenizer.encode("ab")) == "ab"


def test_special_tokens_round_trip_through_decode() -> None:
    vocab = {"a": 0}
    special_tokens = {"<|im_start|>": 1}
    tokenizer = Tokenizer(vocab, merges={}, special_tokens=special_tokens)

    ids = tokenizer.encode("<|im_start|>a")

    assert tokenizer.decode(ids) == "<|im_start|>a"


def test_split_words_groups_contraction_after_a_letter() -> None:
    """"argument's" must split as ["argument", "'s"], matching the real
    tokenizer - not ["argument", "'", "s"], which would feed the model an
    unfamiliar (out-of-distribution) tokenization of ordinary English text
    and measurably degrade accuracy (verified against the real
    Qwen3-0.6B tokenizer; see the "Challenges faced" section of the README).
    """
    assert _split_words("argument's") == ["argument", "'s"]


def test_split_words_does_not_mistake_a_quote_for_a_contraction() -> None:
    """Regression test: an opening single-quote directly followed by a
    letter that happens to spell a contraction suffix (e.g. the "d" in
    "'dog'") must NOT be treated as the contraction "'d" - contractions
    only glue onto the *end* of a preceding word, never the start of a
    quoted one. This exact case ("Substitute ... with 'dog' ...") broke
    real end-to-end runs before this fix.
    """
    assert _split_words("'dog'") == ["'", "dog", "'"]


def test_split_words_keeps_underscored_identifiers_together() -> None:
    assert _split_words("fn_greet") == ["fn_greet"]


def test_split_words_groups_extra_spaces_into_their_own_word() -> None:
    assert _split_words("a  b") == ["a", " ", " b"]
