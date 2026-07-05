"""A from-scratch byte-level BPE tokenizer.

This is the bonus "recoded tokenizer": the main code path never calls the
SDK's own ``encode``/``decode``. Instead, :class:`Tokenizer` is built purely
from ``vocab.json`` (via ``get_path_to_vocab_file``), ``merges.txt`` (via
``get_path_to_merges_file``), and the added/special tokens in
``tokenizer.json`` (via ``get_path_to_tokenizer_file``), and reimplements
the same byte-level BPE algorithm GPT-2/Qwen tokenizers use:

1. Pre-tokenize text into "words" (runs of letters, digits, or other
   characters, with byte-level BPE's leading-space convention).
2. Map each word's raw UTF-8 bytes to the byte-level unicode alphabet.
3. Repeatedly merge the highest-priority adjacent symbol pair according to
   ``merges.txt``, until no more merges apply.
4. Look up each resulting symbol's token id in the vocabulary.

The exact word boundaries this produces do not need to match the SDK's own
tokenizer byte-for-byte: any valid decomposition into vocabulary entries
whose text concatenates back to the original input is correct for our
purposes (constrained decoding only needs the token ids to faithfully
represent the forced literal text, not the *minimal* possible token count).
"""

import re
from typing import Protocol

from src.tokenizer_vocab import byte_to_unicode


class TokenizerBackend(Protocol):
    """What :class:`~src.constrained_decoder.ConstrainedJSONGenerator` needs."""

    def encode(self, text: str) -> list[int]:
        ...


_CONTRACTIONS = ("'s", "'t", "'re", "'ve", "'m", "'ll", "'d")


def _consume_token(text: str, index: int) -> tuple[str, int]:
    """Consume one non-space token starting at ``index`` (assumes no space there).

    Returns the token text and the index right after it: an English
    contraction (``'s``, ``'t``, ...) if one starts here *and* it directly
    follows a letter (contractions glue onto the end of a word, e.g.
    ``argument's`` - never onto a space or punctuation, which rules out
    false positives like the opening quote of ``'dog'`` being mistaken for
    the contraction ``'d``). Otherwise a maximal run of letters, a maximal
    run of digits, or a maximal run of any other single character class -
    matching GPT-2's own pre-tokenizer regex closely enough that real
    vocabulary merges (e.g. for multi-space indentation or punctuation
    clusters) can actually apply.
    """
    follows_letter = index > 0 and text[index - 1].isalpha()
    if follows_letter:
        contraction = next((c for c in _CONTRACTIONS if text.startswith(c, index)), None)
        if contraction is not None:
            return contraction, index + len(contraction)

    length = len(text)
    char = text[index]
    end = index
    if char.isalpha() or char == "_":
        while end < length and (text[end].isalpha() or text[end] == "_"):
            end += 1
    elif char.isdigit():
        while end < length and text[end].isdigit():
            end += 1
    else:
        while end < length and text[end] != " " and not text[end].isalpha() and not text[end].isdigit():
            end += 1
    return text[index:end], end


def _split_words(text: str) -> list[str]:
    """Split text into BPE "words", mirroring GPT-2's own pre-tokenizer regex
    (``'s|'t|'re|'ve|'m|'ll|'d| ?\\w+| ?\\d+| ?[^\\s\\w\\d]+|\\s+``, roughly).

    A run of ``N`` consecutive spaces becomes: the first ``N - 1`` spaces as
    their own combined word (so real multi-space vocabulary entries, e.g.
    for JSON indentation, can actually be used), and the last space folded
    as a prefix onto whatever follows (byte-level BPE's "Ġ" convention).
    Trailing whitespace at the very end of the text is its own word.
    """
    words: list[str] = []
    index = 0
    length = len(text)
    while index < length:
        if text[index] == " ":
            run_start = index
            while index < length and text[index] == " ":
                index += 1
            run_length = index - run_start
            if index == length:
                words.append(" " * run_length)
                continue
            if run_length > 1:
                words.append(" " * (run_length - 1))
            token, index = _consume_token(text, index)
            words.append(" " + token)
            continue
        token, index = _consume_token(text, index)
        words.append(token)
    return words


def _apply_merges(symbols: list[str], merge_ranks: dict[tuple[str, str], int]) -> list[str]:
    """Repeatedly merge the highest-priority adjacent pair until none apply."""
    symbols = list(symbols)
    while len(symbols) > 1:
        best_rank: int | None = None
        best_index = -1
        for index in range(len(symbols) - 1):
            rank = merge_ranks.get((symbols[index], symbols[index + 1]))
            if rank is not None and (best_rank is None or rank < best_rank):
                best_rank = rank
                best_index = index
        if best_index == -1:
            break
        merged = symbols[best_index] + symbols[best_index + 1]
        symbols = symbols[:best_index] + [merged] + symbols[best_index + 2:]
    return symbols


class Tokenizer:
    """A byte-level BPE encoder/decoder built only from vocab + merge rules."""

    def __init__(
        self,
        vocab: dict[str, int],
        merges: dict[tuple[str, str], int],
        special_tokens: dict[str, int] | None = None,
    ) -> None:
        """Initialize the tokenizer.

        Args:
            vocab: Raw (undecoded) ``{token: id}`` mapping, as loaded by
                :func:`src.tokenizer_vocab.load_raw_vocab`.
            merges: Merge rank table, as loaded by
                :func:`src.tokenizer_vocab.load_merges`.
            special_tokens: Chat-template markers (e.g. ``"<|im_start|>"``)
                mapped to their id, as loaded by
                :func:`src.tokenizer_vocab.load_special_tokens`. These live
                *outside* ``vocab`` entirely (real tokenizers keep
                added/special tokens separate from the base byte-level BPE
                vocabulary) and are matched as a single atomic unit, never
                split apart by the merge algorithm.
        """
        self._vocab = vocab
        self._merges = merges
        self._byte_to_unicode = byte_to_unicode()
        self._id_to_token = {token_id: token for token, token_id in vocab.items()}
        self._special_tokens = dict(special_tokens) if special_tokens else {}
        self._special_id_to_content = {token_id: content for content, token_id in self._special_tokens.items()}
        self._special_token_re = (
            re.compile("(" + "|".join(re.escape(token) for token in self._special_tokens) + ")")
            if self._special_tokens
            else None
        )

    def encode(self, text: str) -> list[int]:
        """Encode text into a list of token ids.

        Args:
            text: The text to tokenize.

        Returns:
            The token ids. Falls back to per-byte tokens for any symbol
            that (unexpectedly) is not itself a vocabulary entry, so
            encoding never crashes on unusual input.
        """
        token_ids: list[int] = []
        for chunk in self._split_on_special_tokens(text):
            special_id = self._special_tokens.get(chunk)
            if special_id is not None:
                token_ids.append(special_id)
                continue
            for word in _split_words(chunk):
                raw_bytes = word.encode("utf-8")
                symbols = [self._byte_to_unicode[byte] for byte in raw_bytes]
                for symbol in _apply_merges(symbols, self._merges):
                    token_id = self._vocab.get(symbol)
                    if token_id is not None:
                        token_ids.append(token_id)
                    else:
                        token_ids.extend(self._vocab[char] for char in symbol)
        return token_ids

    def _split_on_special_tokens(self, text: str) -> list[str]:
        """Split ``text`` around any special-token markers, keeping them."""
        if self._special_token_re is None:
            return [text]
        return [part for part in self._special_token_re.split(text) if part]

    def decode(self, token_ids: list[int]) -> str:
        """Decode a list of token ids back into text.

        Args:
            token_ids: The token ids to decode.

        Returns:
            The reconstructed text. Unknown ids are skipped.
        """
        unicode_to_byte = {char: byte for byte, char in self._byte_to_unicode.items()}
        parts: list[str] = []
        byte_values = bytearray()

        def flush() -> None:
            if byte_values:
                parts.append(byte_values.decode("utf-8", errors="replace"))
                byte_values.clear()

        for token_id in token_ids:
            special_content = self._special_id_to_content.get(token_id)
            if special_content is not None:
                flush()
                parts.append(special_content)
                continue
            token = self._id_to_token.get(token_id)
            if token is None:
                continue
            for char in token:
                byte = unicode_to_byte.get(char)
                if byte is not None:
                    byte_values.append(byte)
        flush()
        return "".join(parts)
