"""Retrieval over the persisted BM25 index."""
import pickle
from pathlib import Path
from typing import List

from rank_bm25 import BM25Okapi

from src.indexer import INDEX_FILENAME
from src.models import Chunk, MinimalSource
from src.tokenizer import tokenize


class Retriever:
    """Loads the BM25 index and answers top-k queries."""

    def __init__(self, processed_dir: str = "data/processed") -> None:
        """Load the persisted index into memory.

        Args:
            processed_dir: Directory containing the index file.

        Raises:
            FileNotFoundError: If the index has not been built yet.
        """
        index_path = Path(processed_dir) / INDEX_FILENAME
        if not index_path.exists():
            raise FileNotFoundError(
                f"Index not found at {index_path}. Run `index` first."
            )
        with index_path.open("rb") as handle:
            data = pickle.load(handle)
        self.chunks: List[Chunk] = data["chunks"]
        self.bm25: BM25Okapi = data["bm25"]

    def search(self, query: str, k: int) -> List[MinimalSource]:
        """Return the top-k source locations for a query.

        Args:
            query: The natural-language question.
            k: Number of results to return.

        Returns:
            Up to k source locations, ranked by BM25 score. Empty if
            the query is empty or k <= 0.
        """
        if k <= 0 or not query.strip():
            return []

        query_tokens = tokenize(query)
        if not query_tokens:
            return []

        scores = self.bm25.get_scores(query_tokens)
        # Indices of the top-k scoring chunks, highest first.
        ranked = sorted(
            range(len(scores)), key=lambda i: scores[i], reverse=True
        )[:k]

        results: List[MinimalSource] = []
        for i in ranked:
            chunk = self.chunks[i]
            results.append(
                MinimalSource(
                    file_path=chunk.file_path,
                    first_character_index=chunk.first_character_index,
                    last_character_index=chunk.last_character_index,
                )
            )
        return results
    