"""CLI entry point for the RAG pipeline."""
import fire
from tqdm import tqdm

from src.indexer import build_index
from src.io_utils import load_dataset, save_search_results
from src.models import MinimalSearchResults
from src.retriever import Retriever


class RagCLI:
    """Command-line interface for the RAG system."""

    def index(
        self,
        max_chunk_size: int = 2000,
        docs_chunk_size: int = 1000,
        raw_dir: str = "data/raw",
        processed_dir: str = "data/processed",
    ) -> None:
        """Ingest the corpus and build the index under data/processed/.

        Args:
            max_chunk_size: Maximum chunk length for code (CLI ceiling).
            docs_chunk_size: Maximum chunk length for docs/text files.
            raw_dir: Directory containing the raw corpus.
            processed_dir: Directory where the index is written.
        """
        build_index(raw_dir, processed_dir, max_chunk_size, docs_chunk_size)

    def search(
        self,
        query: str,
        k: int = 5,
        processed_dir: str = "data/processed",
    ) -> None:
        """Return and print the top-k sources for a single query.

        Args:
            query: The question to search for.
            k: Number of results to return.
            processed_dir: Directory containing the index.
        """
        retriever = Retriever(processed_dir)
        for source in retriever.search(query, k):
            print(
                f"{source.file_path} "
                f"[{source.first_character_index}:"
                f"{source.last_character_index}]"
            )

    def search_dataset(
        self,
        dataset_path: str,
        k: int = 10,
        save_directory: str = "data/output/search_results",
        processed_dir: str = "data/processed",
    ) -> None:
        """Run search over a whole dataset and save the results.

        Args:
            dataset_path: Path to the questions dataset JSON.
            k: Number of results per question.
            save_directory: Directory to write the output into.
            processed_dir: Directory containing the index.
        """
        dataset = load_dataset(dataset_path)
        retriever = Retriever(processed_dir)
        results = []
        for question in tqdm(
            dataset.rag_questions, desc="Searching", unit="q"
        ):
            sources = retriever.search(question.question, k)
            results.append(
                MinimalSearchResults(
                    question_id=question.question_id,
                    question=question.question,
                    retrieved_sources=sources,
                )
            )
        out_path = save_search_results(
            results, k, dataset_path, save_directory
        )
        print(f"Saved student_search_results to {out_path}")

    def evaluate(
        self,
        student_search_results_path: str,
        dataset_path: str,
        k: int = 5,
    ) -> None:
        """Report your own recall@k against a ground-truth dataset.

        Args:
            student_search_results_path: Path to your search results.
            dataset_path: Path to the ground-truth AnsweredQuestions.
            k: Cutoff for recall.
        """
        from src.evaluator import evaluate as run_eval

        run_eval(student_search_results_path, dataset_path, k)


def main() -> None:
    """Launch the Fire CLI."""
    fire.Fire(RagCLI)


if __name__ == "__main__":
    main()
