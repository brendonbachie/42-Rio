"""Corpus indexing: read files, chunk them, build and persist a BM25 index."""
import pickle
import time
from pathlib import Path
from typing import List, Tuple

from rank_bm25 import BM25Okapi
from tqdm import tqdm

from src.chunking import chunk_markdown, chunk_python
from src.models import Chunk
from src.tokenizer import tokenize

INDEX_FILENAME = "index.pkl"


def _iter_corpus_files(raw_dir: Path) -> List[Path]:
    """Collect all indexable files under a directory.

    Args:
        raw_dir: Root directory of the raw corpus.

    Returns:
        Sorted list of indexable file paths (deduplicated).
    """
    patterns = ("*.py", "*.md", "*.txt", "*.rst", "CMakeLists.txt", "*.cmake")
    files: List[Path] = []
    for pattern in patterns:
        files.extend(raw_dir.rglob(pattern))
    return sorted(set(files))


def _read_text(path: Path) -> str:
    """Read a file as UTF-8, tolerating decode errors.

    Args:
        path: File to read.

    Returns:
        File content, with undecodable bytes replaced.
    """
    with path.open("r", encoding="utf-8", errors="replace") as handle:
        return handle.read()


def _chunk_file(
    path: Path,
    text: str,
    code_chunk_size: int,
    docs_chunk_size: int,
) -> List[Chunk]:
    """Dispatch to the right chunker based on file extension.

    Python files use the code chunker with the larger size so whole
    functions stay together; everything else uses the text chunker
    with the smaller docs size for finer granularity.

    Args:
        path: File path (used for extension and as the chunk file_path).
        text: File content.
        code_chunk_size: Maximum chunk length for Python files.
        docs_chunk_size: Maximum chunk length for text/Markdown files.

    Returns:
        The chunks for this file.
    """
    file_path = path.as_posix()
    if path.suffix == ".py":
        return chunk_python(text, file_path, code_chunk_size)
    return chunk_markdown(text, file_path, docs_chunk_size)


def build_index(
    raw_dir: str = "data/raw",
    processed_dir: str = "data/processed",
    max_chunk_size: int = 2000,
    docs_chunk_size: int = 1000,
) -> Tuple[int, int]:
    """Build the BM25 index over the corpus and persist it.

    Args:
        raw_dir: Directory containing the raw corpus.
        processed_dir: Directory where the index is written.
        max_chunk_size: Maximum chunk length for code (the CLI ceiling).
        docs_chunk_size: Maximum chunk length for docs/text files.

    Returns:
        A tuple (number of files indexed, number of chunks produced).

    Raises:
        FileNotFoundError: If the corpus directory does not exist.
    """
    start_time = time.time()
    raw_path = Path(raw_dir)
    if not raw_path.exists():
        raise FileNotFoundError(f"Corpus directory not found: {raw_dir}")

    # Docs never exceed the CLI ceiling, even if a larger value is passed.
    docs_size = min(docs_chunk_size, max_chunk_size)

    files = _iter_corpus_files(raw_path)
    all_chunks: List[Chunk] = []

    for path in tqdm(files, desc="Chunking", unit="file"):
        text = _read_text(path)
        all_chunks.extend(
            _chunk_file(path, text, max_chunk_size, docs_size)
        )

    if not all_chunks:
        print(f"No indexable files found under {raw_dir}/. Nothing to index.")
        return len(files), 0

    tokenized_corpus: List[List[str]] = [
        tokenize(chunk.text)
        for chunk in tqdm(all_chunks, desc="Tokenizing", unit="chunk")
    ]

    bm25 = BM25Okapi(tokenized_corpus)

    out_dir = Path(processed_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    with (out_dir / INDEX_FILENAME).open("wb") as handle:
        pickle.dump(
            {"chunks": all_chunks, "bm25": bm25},
            handle,
            protocol=pickle.HIGHEST_PROTOCOL,
        )

    elapsed = time.time() - start_time
    print(
        f"Ingestion complete! Indexed {len(all_chunks)} chunks "
        f"under {processed_dir}/ in {elapsed:.1f}s"
    )
    return len(files), len(all_chunks)
