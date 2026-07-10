"""Pydantic data models exchanged between pipeline stages."""
import uuid
from typing import List, Union

from pydantic import BaseModel, Field


class MinimalSource(BaseModel):
    """A single source location within the corpus.

    Attributes:
        file_path: Path to the source file, written exactly as in the
            ingested corpus (compared verbatim by the grader).
        first_character_index: Starting character position in the file.
        last_character_index: Ending character position in the file.
    """

    file_path: str
    first_character_index: int
    last_character_index: int


class UnansweredQuestion(BaseModel):
    """A question that has not yet been answered.

    Attributes:
        question_id: Unique identifier, auto-generated if not provided.
        question: The natural-language question text.
    """

    question_id: str = Field(default_factory=lambda: str(uuid.uuid4()))
    question: str


class AnsweredQuestion(UnansweredQuestion):
    """A question with its ground-truth sources and answer.

    Attributes:
        sources: The correct source locations for this question.
        answer: The reference answer text.
    """

    sources: List[MinimalSource]
    answer: str


class RagDataset(BaseModel):
    """A dataset of RAG questions (answered or unanswered).

    Attributes:
        rag_questions: The list of questions in the dataset.
    """

    rag_questions: List[Union[AnsweredQuestion, UnansweredQuestion]]


class MinimalSearchResults(BaseModel):
    """Retrieved sources for a single question.

    Attributes:
        question_id: Identifier of the question searched.
        question: The question text.
        retrieved_sources: The top-k source locations retrieved.
    """

    question_id: str
    question: str
    retrieved_sources: List[MinimalSource]


class MinimalAnswer(MinimalSearchResults):
    """Search results enriched with a generated answer.

    Attributes:
        answer: The generated natural-language answer.
    """

    answer: str


class StudentSearchResults(BaseModel):
    """Output of a batch search over a dataset.

    Attributes:
        search_results: One entry per question searched.
        k: Number of results requested per question.
    """

    search_results: List[MinimalSearchResults]
    k: int


class StudentSearchResultsAndAnswer(BaseModel):
    """Output of batch answer generation over a dataset.

    Attributes:
        search_results: One entry per question, with its answer.
        k: Number of results requested per question.
    """

    search_results: List[MinimalAnswer]
    k: int


class Chunk(BaseModel):
    """A chunk of a source file, with its character span in the original.

    Attributes:
        file_path: Path to the source file, exactly as in the corpus.
        text: The chunk's textual content.
        first_character_index: Start offset in the original file.
        last_character_index: End offset in the original file.
    """

    file_path: str
    text: str
    first_character_index: int
    last_character_index: int
