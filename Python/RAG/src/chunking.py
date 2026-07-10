"""Chunking strategies for Python code and Markdown/text files."""
import ast
from typing import List

from src.models import Chunk


def _split_long_text(
    text: str,
    file_path: str,
    base_offset: int,
    max_chunk_size: int,
    overlap: int = 400,
) -> List[Chunk]:
    """Split a text span into overlapping chunks.

    Consecutive chunks share `overlap` characters so that content near
    a boundary still appears whole in at least one chunk.

    Args:
        text: The text span to split.
        file_path: Source file path, propagated to each chunk.
        base_offset: Offset of `text` within the original file.
        max_chunk_size: Maximum chunk length in characters.
        overlap: Number of characters shared between adjacent chunks.

    Returns:
        A list of chunks covering the input span.
    """
    chunks: List[Chunk] = []
    n = len(text)
    step = max(1, max_chunk_size - overlap)
    start = 0
    while start < n:
        end = min(start + max_chunk_size, n)
        if end < n:
            newline = text.rfind("\n", start, end)
            if newline > start:
                end = newline
        chunk_text = text[start:end]
        if chunk_text.strip():
            chunks.append(
                Chunk(
                    file_path=file_path,
                    text=chunk_text,
                    first_character_index=base_offset + start,
                    last_character_index=base_offset + end,
                )
            )
        if end >= n:
            break
        start += step
    return chunks


def chunk_markdown(
    text: str,
    file_path: str,
    max_chunk_size: int,
) -> List[Chunk]:
    """Chunk a Markdown/text document by header sections.

    Each section starts at a line beginning with '#'. Sections longer
    than max_chunk_size are further split by `_split_long_text`.

    Args:
        text: Full file content.
        file_path: Source file path.
        max_chunk_size: Maximum chunk length in characters.

    Returns:
        A list of chunks with absolute character spans.
    """
    chunks: List[Chunk] = []
    lines = text.splitlines(keepends=True)

    section_start = 0
    offset = 0
    current_len = 0

    def flush(section_end: int) -> None:
        if current_len == 0:
            return
        section_text = text[section_start:section_end]
        if len(section_text) <= max_chunk_size:
            if section_text.strip():
                chunks.append(
                    Chunk(
                        file_path=file_path,
                        text=section_text,
                        first_character_index=section_start,
                        last_character_index=section_end,
                    )
                )
        else:
            chunks.extend(
                _split_long_text(
                    section_text, file_path, section_start, max_chunk_size
                )
            )

    for line in lines:
        is_header = line.lstrip().startswith("#")
        if is_header and current_len > 0:
            flush(offset)
            section_start = offset
            current_len = 0
        offset += len(line)
        current_len += len(line)

    flush(offset)
    return chunks


def _line_offsets(text: str) -> List[int]:
    """Return the character offset at the start of each line.

    Args:
        text: The full file content.

    Returns:
        A list where element i is the offset of line i (0-indexed).
    """
    offsets = [0]
    for line in text.splitlines(keepends=True):
        offsets.append(offsets[-1] + len(line))
    return offsets


def chunk_python(
    text: str,
    file_path: str,
    max_chunk_size: int,
) -> List[Chunk]:
    """Chunk a Python file by top-level functions and classes.

    Falls back to plain text splitting if the file cannot be parsed
    (e.g. invalid syntax). Each top-level definition becomes a chunk;
    oversized definitions are further split by `_split_long_text`.

    Args:
        text: Full file content.
        file_path: Source file path.
        max_chunk_size: Maximum chunk length in characters.

    Returns:
        A list of chunks with absolute character spans.
    """
    try:
        tree = ast.parse(text)
    except SyntaxError:
        return _split_long_text(text, file_path, 0, max_chunk_size)

    offsets = _line_offsets(text)
    chunks: List[Chunk] = []

    for node in tree.body:
        if not isinstance(
            node, (ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)
        ):
            continue
        start_line = node.lineno - 1
        end_line = getattr(node, "end_lineno", None)
        if end_line is None:
            continue
        start = offsets[start_line]
        end = offsets[end_line] if end_line < len(offsets) else len(text)
        span_text = text[start:end]
        if len(span_text) <= max_chunk_size:
            if span_text.strip():
                chunks.append(
                    Chunk(
                        file_path=file_path,
                        text=span_text,
                        first_character_index=start,
                        last_character_index=end,
                    )
                )
        else:
            chunks.extend(
                _split_long_text(span_text, file_path, start, max_chunk_size)
            )

    if not chunks:
        return _split_long_text(text, file_path, 0, max_chunk_size)

    return chunks
