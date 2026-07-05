"""Vocabulary and merge-rule loading for a from-scratch byte-level BPE tokenizer.

The LLM SDK only exposes ``get_logits_from_input_ids``, ``get_path_to_vocab_file``
and ``get_path_to_merges_file`` for low-level access (no private tokenizer
internals). This module loads the raw byte-level BPE ``vocab.json`` and
``merges.txt`` files (the same scheme used by GPT-2 and Qwen tokenizers) in
two complementary forms:

* :func:`load_vocab` decodes every token to its real text, for the grammar
  checks driving constrained decoding (see ``src/constrained_decoder.py``).
* :func:`load_raw_vocab` and :func:`load_merges` keep the raw, undecoded
  representation, for :mod:`src.tokenizer`'s own BPE encoder/decoder - which
  is what actually turns text into token ids without ever calling the SDK's
  own ``encode``/``decode``.
"""

import json
from functools import lru_cache
from pathlib import Path


class VocabLoadError(RuntimeError):
    """Raised when the vocabulary or merges file cannot be loaded or parsed."""


@lru_cache(maxsize=1)
def byte_to_unicode() -> dict[int, str]:
    """Build the standard GPT-2 byte-level BPE byte<->unicode mapping.

    Returns:
        Mapping of raw byte value (0-255) to the single unicode character
        used to represent it inside a byte-level BPE vocabulary file.
    """
    bs = (
        list(range(ord("!"), ord("~") + 1))
        + list(range(ord("¡"), ord("¬") + 1))
        + list(range(ord("®"), ord("ÿ") + 1))
    )
    cs = list(bs)
    shift = 0
    for byte in range(256):
        if byte not in bs:
            bs.append(byte)
            cs.append(256 + shift)
            shift += 1
    return {byte: chr(code) for byte, code in zip(bs, cs)}


def _decode_token(token_str: str, unicode_to_byte: dict[str, int]) -> str:
    """Decode a single byte-level BPE token string back to real text.

    Args:
        token_str: The raw key from ``vocab.json``.
        unicode_to_byte: Reverse of :func:`byte_to_unicode`.

    Returns:
        The decoded text. Falls back to the raw token string when a
        character is not part of the byte-level mapping (e.g. special
        tokens such as ``<|im_start|>``).
    """
    byte_values = bytearray()
    for char in token_str:
        byte = unicode_to_byte.get(char)
        if byte is None:
            return token_str
        byte_values.append(byte)
    return bytes(byte_values).decode("utf-8", errors="replace")


def _read_raw_vocab(vocab_path: str) -> dict[str, int]:
    """Read ``vocab.json`` and return its raw ``{token: id}`` content.

    Raises:
        VocabLoadError: If the file is missing, unreadable, or not valid
            JSON in the expected ``{token: id}`` format.
    """
    path = Path(vocab_path)
    try:
        raw_text = path.read_text(encoding="utf-8")
    except OSError as exc:
        raise VocabLoadError(f"Vocabulary file not found: {vocab_path}") from exc

    try:
        raw = json.loads(raw_text)
    except json.JSONDecodeError as exc:
        raise VocabLoadError(f"Vocabulary file is not valid JSON: {vocab_path}") from exc

    if not isinstance(raw, dict):
        raise VocabLoadError(f"Vocabulary file has an unexpected format: {vocab_path}")

    return {
        token_str: token_id
        for token_str, token_id in raw.items()
        if isinstance(token_str, str) and isinstance(token_id, int)
    }


def load_vocab(vocab_path: str) -> dict[int, str]:
    """Load a byte-level BPE ``vocab.json`` file and decode tokens to text.

    Args:
        vocab_path: Path to the vocabulary JSON file (token string -> id).

    Returns:
        Mapping of token id to its decoded text representation.

    Raises:
        VocabLoadError: If the file is missing, unreadable, or not valid
            JSON in the expected ``{token: id}`` format.
    """
    unicode_to_byte = {char: byte for byte, char in byte_to_unicode().items()}
    return {
        token_id: _decode_token(token_str, unicode_to_byte)
        for token_str, token_id in _read_raw_vocab(vocab_path).items()
    }


def load_raw_vocab(vocab_path: str) -> dict[str, int]:
    """Load ``vocab.json`` in its raw, undecoded form: ``{token: id}``.

    This is what the from-scratch BPE encoder in :mod:`src.tokenizer` uses
    to look up merged byte-level symbols, since merge rules and vocabulary
    keys are expressed in the same undecoded alphabet.

    Args:
        vocab_path: Path to the vocabulary JSON file.

    Returns:
        The raw ``{token: id}`` mapping.

    Raises:
        VocabLoadError: If the file is missing, unreadable, or not valid
            JSON in the expected ``{token: id}`` format.
    """
    return _read_raw_vocab(vocab_path)


def load_merges(merges_path: str) -> dict[tuple[str, str], int]:
    """Load a byte-level BPE ``merges.txt`` file into a rank table.

    Args:
        merges_path: Path to the merges file. Each non-comment, non-empty
            line contains two space-separated symbols that may be merged;
            earlier lines have higher merge priority.

    Returns:
        Mapping of ``(left, right)`` symbol pairs to their merge rank (lower
        rank = higher priority).

    Raises:
        VocabLoadError: If the file is missing or unreadable.
    """
    path = Path(merges_path)
    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except OSError as exc:
        raise VocabLoadError(f"Merges file not found: {merges_path}") from exc

    ranks: dict[tuple[str, str], int] = {}
    for line in lines:
        if not line or line.startswith("#"):
            continue
        parts = line.split(" ")
        if len(parts) != 2:
            continue
        pair = (parts[0], parts[1])
        ranks.setdefault(pair, len(ranks))
    return ranks


def load_special_tokens(tokenizer_path: str) -> dict[str, int]:
    """Load added/special tokens (e.g. chat-template markers) from ``tokenizer.json``.

    Tokens such as ``<|im_start|>``/``<|im_end|>`` are *not* part of the
    regular byte-level BPE vocabulary (``vocab.json``): they live in
    ``tokenizer.json``'s ``added_tokens`` list instead, each with its own id
    outside the base vocabulary, and must be matched as a single atomic
    unit rather than run through the normal merge algorithm.

    Args:
        tokenizer_path: Path to ``tokenizer.json``.

    Returns:
        Mapping of special token text to its id. Empty if the file has no
        ``added_tokens`` section.

    Raises:
        VocabLoadError: If the file is missing, unreadable, or not valid
            JSON.
    """
    path = Path(tokenizer_path)
    try:
        raw_text = path.read_text(encoding="utf-8")
    except OSError as exc:
        raise VocabLoadError(f"Tokenizer file not found: {tokenizer_path}") from exc

    try:
        data = json.loads(raw_text)
    except json.JSONDecodeError as exc:
        raise VocabLoadError(f"Tokenizer file is not valid JSON: {tokenizer_path}") from exc

    added_tokens = data.get("added_tokens", []) if isinstance(data, dict) else []
    if not isinstance(added_tokens, list):
        return {}

    special_tokens: dict[str, int] = {}
    for entry in added_tokens:
        if isinstance(entry, dict) and isinstance(entry.get("content"), str) and isinstance(entry.get("id"), int):
            special_tokens[entry["content"]] = entry["id"]
    return special_tokens
