"""Answer generation with Qwen/Qwen3-0.6B over retrieved context."""
from typing import List

import torch
from transformers import AutoModelForCausalLM, AutoTokenizer

from src.models import MinimalSource

DEFAULT_MODEL = "Qwen/Qwen3-0.6B"
# Character budget for the concatenated context (keeps us within the
# model's token window with margin; ~4 chars per token heuristic).
CONTEXT_CHAR_BUDGET = 6000
MAX_NEW_TOKENS = 256


class Generator:
    """Loads Qwen once and answers questions from retrieved context."""

    def __init__(self, model_name: str = DEFAULT_MODEL) -> None:
        """Load the tokenizer and model onto CPU.

        Args:
            model_name: HuggingFace model identifier.
        """
        self.tokenizer = AutoTokenizer.from_pretrained(model_name)
        self.model = AutoModelForCausalLM.from_pretrained(
            model_name,
            dtype=torch.float32,
        )
        self.model.eval()

    def _build_context(
        self,
        sources: List[MinimalSource],
        source_texts: List[str],
    ) -> str:
        """Concatenate source snippets within the character budget.

        Args:
            sources: The retrieved source locations (for their paths).
            source_texts: The text of each retrieved chunk, aligned with
                `sources`.

        Returns:
            A context string labelled by source file.
        """
        parts: List[str] = []
        total = 0
        for source, text in zip(sources, source_texts):
            snippet = f"# Source: {source.file_path}\n{text}\n"
            if total + len(snippet) > CONTEXT_CHAR_BUDGET:
                break
            parts.append(snippet)
            total += len(snippet)
        return "\n".join(parts)

    def answer(
        self,
        question: str,
        sources: List[MinimalSource],
        source_texts: List[str],
    ) -> str:
        """Generate an answer grounded in the retrieved sources.

        Args:
            question: The user question.
            sources: The retrieved source locations.
            source_texts: The text of each retrieved chunk.

        Returns:
            The generated answer text.
        """
        if not source_texts:
            return "No relevant sources were found to answer this question."

        context = self._build_context(sources, source_texts)
        system = (
            "You are a helpful assistant that answers questions about a "
            "codebase. Answer ONLY using the provided sources. If the "
            "sources do not contain the answer, say so. Be concise."
        )
        user = f"Sources:\n{context}\n\nQuestion: {question}\n\nAnswer:"

        messages = [
            {"role": "system", "content": system},
            {"role": "user", "content": user},
        ]
        prompt = self.tokenizer.apply_chat_template(
            messages,
            tokenize=False,
            add_generation_prompt=True,
            enable_thinking=False,
        )
        inputs = self.tokenizer(prompt, return_tensors="pt")

        with torch.no_grad():
            output = self.model.generate(   # type: ignore[misc]
                **inputs,
                max_new_tokens=MAX_NEW_TOKENS,
                do_sample=False,
                pad_token_id=self.tokenizer.eos_token_id,
            )
        generated = output[0][inputs["input_ids"].shape[1]:]
        text = self.tokenizer.decode(generated, skip_special_tokens=True)
        return str(text).strip()


def read_source_text(source: MinimalSource) -> str:
    """Read the text span a source points to, from the original file.

    Args:
        source: The source location to read.

    Returns:
        The referenced text, or an empty string if the file is missing
        or the range is invalid.
    """
    try:
        with open(source.file_path,
                  "r", encoding="utf-8", errors="replace") as handle:
            content = handle.read()
        return content[
            source.first_character_index:source.last_character_index
        ]
    except OSError:
        return ""
