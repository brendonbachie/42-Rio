"""Recall@k evaluation against a ground-truth dataset."""
import json
from pathlib import Path
from typing import Dict, List

from src.models import (
    AnsweredQuestion,
    MinimalSource,
    RagDataset,
    StudentSearchResults,
)


def _overlaps(a: MinimalSource, b: MinimalSource) -> bool:
    """Return True if two sources cover the same file and overlap.

    Uses a low IoU-style overlap: any character-range intersection in
    the same file counts as a hit.

    Args:
        a: A retrieved source.
        b: A ground-truth source.

    Returns:
        Whether `a` covers `b`'s region in the same file.
    """
    if a.file_path != b.file_path:
        return False
    start = max(a.first_character_index, b.first_character_index)
    end = min(a.last_character_index, b.last_character_index)
    return end > start


def _recall_for_question(
    retrieved: List[MinimalSource],
    truth: List[MinimalSource],
    k: int,
) -> float:
    """Fraction of ground-truth sources covered by the top-k retrieved.

    Args:
        retrieved: The retrieved sources (already ranked).
        truth: The ground-truth sources.
        k: Cutoff for the retrieved list.

    Returns:
        Recall in [0, 1] for this question.
    """
    if not truth:
        return 0.0
    top = retrieved[:k]
    found = sum(
        1 for t in truth if any(_overlaps(r, t) for r in top)
    )
    return found / len(truth)


def evaluate(
    student_search_results_path: str,
    dataset_path: str,
    k: int = 5,
) -> Dict[str, float]:
    """Compute mean recall@k over a dataset.

    Args:
        student_search_results_path: Path to your search results JSON.
        dataset_path: Path to the ground-truth AnsweredQuestions JSON.
        k: Cutoff for recall.

    Returns:
        A dict with the mean recall@k.
    """
    with Path(student_search_results_path).open(encoding="utf-8") as handle:
        student = StudentSearchResults.model_validate(json.load(handle))
    with Path(dataset_path).open(encoding="utf-8") as handle:
        truth_ds = RagDataset.model_validate(json.load(handle))

    truth_by_id: Dict[str, List[MinimalSource]] = {}
    for q in truth_ds.rag_questions:
        if isinstance(q, AnsweredQuestion):
            truth_by_id[q.question_id] = q.sources

    recalls: List[float] = []
    for result in student.search_results:
        truth = truth_by_id.get(result.question_id)
        if truth is None:
            continue
        recalls.append(
            _recall_for_question(result.retrieved_sources, truth, k)
        )

    mean = sum(recalls) / len(recalls) if recalls else 0.0
    print(f"Questions evaluated: {len(recalls)}")
    print(f"Recall@{k}: {mean:.3f} ({mean * 100:.1f}%)")
    return {"recall_at_k": mean}
