"""Tests for byte-level BPE vocabulary loading."""

import json
from pathlib import Path

import pytest

from src.tokenizer_vocab import VocabLoadError, load_merges, load_raw_vocab, load_special_tokens, load_vocab


def test_load_vocab_decodes_plain_ascii(tmp_path: Path) -> None:
    vocab_file = tmp_path / "vocab.json"
    vocab_file.write_text(json.dumps({"hello": 0, "world": 1}), encoding="utf-8")

    result = load_vocab(str(vocab_file))

    assert result[0] == "hello"
    assert result[1] == "world"


def test_load_vocab_decodes_leading_space_marker(tmp_path: Path) -> None:
    vocab_file = tmp_path / "vocab.json"
    vocab_file.write_text(json.dumps({"Ġhello": 5}), encoding="utf-8")

    result = load_vocab(str(vocab_file))

    assert result[5] == " hello"


def test_load_vocab_missing_file_raises_clear_error(tmp_path: Path) -> None:
    with pytest.raises(VocabLoadError):
        load_vocab(str(tmp_path / "does_not_exist.json"))


def test_load_vocab_invalid_json_raises_clear_error(tmp_path: Path) -> None:
    vocab_file = tmp_path / "vocab.json"
    vocab_file.write_text("{not valid json", encoding="utf-8")

    with pytest.raises(VocabLoadError):
        load_vocab(str(vocab_file))


def test_load_raw_vocab_keeps_undecoded_keys(tmp_path: Path) -> None:
    vocab_file = tmp_path / "vocab.json"
    vocab_file.write_text(json.dumps({"Ġhello": 5}), encoding="utf-8")

    result = load_raw_vocab(str(vocab_file))

    assert result == {"Ġhello": 5}


def test_load_merges_ranks_by_line_order(tmp_path: Path) -> None:
    merges_file = tmp_path / "merges.txt"
    merges_file.write_text("#version: 0.2\na b\nb c\n", encoding="utf-8")

    ranks = load_merges(str(merges_file))

    assert ranks == {("a", "b"): 0, ("b", "c"): 1}


def test_load_merges_missing_file_raises_clear_error(tmp_path: Path) -> None:
    with pytest.raises(VocabLoadError):
        load_merges(str(tmp_path / "missing.txt"))


def test_load_special_tokens_reads_added_tokens(tmp_path: Path) -> None:
    tokenizer_file = tmp_path / "tokenizer.json"
    added_tokens = [
        {"id": 151644, "content": "<|im_start|>"},
        {"id": 151645, "content": "<|im_end|>"},
    ]
    tokenizer_file.write_text(json.dumps({"added_tokens": added_tokens}), encoding="utf-8")

    special_tokens = load_special_tokens(str(tokenizer_file))

    assert special_tokens == {"<|im_start|>": 151644, "<|im_end|>": 151645}


def test_load_special_tokens_missing_section_returns_empty(tmp_path: Path) -> None:
    tokenizer_file = tmp_path / "tokenizer.json"
    tokenizer_file.write_text(json.dumps({}), encoding="utf-8")

    assert load_special_tokens(str(tokenizer_file)) == {}
