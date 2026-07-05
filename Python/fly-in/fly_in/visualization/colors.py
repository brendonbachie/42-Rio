"""Mapping from the map file's free-form color names to ANSI escape codes."""

from __future__ import annotations

from typing import Optional

_ANSI_CODES = {
    "black": "30",
    "red": "31",
    "green": "32",
    "yellow": "33",
    "blue": "34",
    "magenta": "35",
    "purple": "35",
    "cyan": "36",
    "white": "37",
    "gray": "90",
    "grey": "90",
    "orange": "38;5;208",
    "brown": "38;5;94",
    "gold": "38;5;220",
    "lime": "38;5;154",
    "crimson": "38;5;196",
    "maroon": "38;5;88",
    "darkred": "38;5;88",
    "violet": "38;5;177",
    "pink": "38;5;213",
    "rainbow": "38;5;201",
}
_RESET = "\033[0m"


def colorize(text: str, color: Optional[str]) -> str:
    """Wrap ``text`` in ANSI codes for ``color``, if it is a known color."""
    if not color:
        return text
    code = _ANSI_CODES.get(color.lower())
    if code is None:
        return text
    return f"\033[{code}m{text}{_RESET}"
