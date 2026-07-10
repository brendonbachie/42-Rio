"""Tokenization shared by indexing and querying.

The same function must be used at index time and query time, otherwise
tokens won't match. It lowercases, splits on non-alphanumerics, and
further splits identifiers (camelCase / snake_case) into sub-words while
keeping the original token too.
"""
import re
from typing import List

_TOKEN_RE = re.compile(r"[A-Za-z0-9]+")
_CAMEL_RE = re.compile(r"[A-Z]?[a-z]+|[A-Z]+(?![a-z])|[0-9]+")


def tokenize(text: str) -> List[str]:
    """Tokenize text into lowercase lexical units.

    Args:
        text: The raw text to tokenize.

    Returns:
        A list of tokens. Compound identifiers yield both the whole
        token and its sub-words (e.g. 'load_lora' -> 'load_lora',
        'load', 'lora').
    """
    tokens: List[str] = []
    for raw in _TOKEN_RE.findall(text):
        lowered = raw.lower()
        tokens.append(lowered)
        subwords = _CAMEL_RE.findall(raw)
        if len(subwords) > 1:
            tokens.extend(sw.lower() for sw in subwords)
    return tokens
