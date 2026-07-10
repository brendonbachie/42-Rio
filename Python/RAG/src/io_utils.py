"""Loading datasets and writing pipeline outputs."""
import json
from pathlib import Path
from typing import List

from src.models import (
    MinimalSearchResults,
    RagDataset,
    StudentSearchResults,
)


def load_dataset(dataset_path: str) -> RagDataset:
    """Load and validate a RAG dataset from JSON.

    Args:
        dataset_path: Path to the dataset JSON file.

    Returns:
        The parsed dataset.

    Raises:
        FileNotFoundError: If the file does not exist.
        ValueError: If the JSON is malformed or fails validation.
    """
    path = Path(dataset_path)
    if not path.exists():
        raise FileNotFoundError(f"Dataset not found: {dataset_path}")
    try:
        with path.open("r", encoding="utf-8") as handle:
            raw = json.load(handle)
        return RagDataset.model_validate(raw)
    except json.JSONDecodeError as exc:
        raise ValueError(f"Malformed JSON in {dataset_path}: {exc}") from exc


def save_search_results(
    results: List[MinimalSearchResults],
    k: int,
    dataset_path: str,
    save_directory: str,
) -> str:
    """Write a StudentSearchResults JSON file.

    The output filename mirrors the input dataset filename so that
    docs/code datasets stay separated.

    Args:
        results: Per-question search results.
        k: Number of results requested per question.
        dataset_path: Original dataset path (used for the filename).
        save_directory: Directory to write the output into.

    Returns:
        The path of the written file.
    """
    out_dir = Path(save_directory)
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / Path(dataset_path).name
    payload = StudentSearchResults(search_results=results, k=k)
    with out_path.open("w", encoding="utf-8") as handle:
        handle.write(payload.model_dump_json(indent=2))
    return str(out_path)
