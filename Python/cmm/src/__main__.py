"""Command-line entry point for the function-calling pipeline.

Usage:
    uv run python -m src [--functions_definition FILE] [--input FILE] [--output FILE] [--model NAME]
"""

from __future__ import annotations

import argparse
import sys

from llm_sdk import Small_LLM_Model

from src.constrained_decoder import ConstrainedJSONGenerator, GenerationError, VocabIndex
from src.function_caller import FunctionCaller
from src.io_utils import ProjectIOError, load_function_definitions, load_test_prompts, save_results
from src.models import FunctionCallResult
from src.tokenizer import Tokenizer
from src.tokenizer_vocab import VocabLoadError, load_merges, load_raw_vocab, load_special_tokens, load_vocab

_DEFAULT_MODEL = "Qwen/Qwen3-0.6B"
_DEFAULT_FUNCTIONS_FILE = "data/input/functions_definition.json"
_DEFAULT_INPUT_FILE = "data/input/function_calling_tests.json"
_DEFAULT_OUTPUT_FILE = "data/output/function_calling_results.json"


def _parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    """Parse command-line arguments for the pipeline."""
    parser = argparse.ArgumentParser(
        prog="python -m src",
        description="Translate natural-language prompts into structured function calls.",
    )
    parser.add_argument(
        "--functions_definition",
        default=_DEFAULT_FUNCTIONS_FILE,
        help="Path to the functions definition JSON file.",
    )
    parser.add_argument(
        "--input",
        default=_DEFAULT_INPUT_FILE,
        help="Path to the natural-language prompts JSON file.",
    )
    parser.add_argument(
        "--output",
        default=_DEFAULT_OUTPUT_FILE,
        help="Path to write the resulting JSON file to.",
    )
    parser.add_argument(
        "--model",
        default=_DEFAULT_MODEL,
        help="Hugging Face model identifier to use (default: %(default)s).",
    )
    parser.add_argument(
        "--trace",
        action="store_true",
        help="Print each constrained-decoding step (candidates considered, token chosen) to stderr.",
    )
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    """Run the full function-calling pipeline.

    Args:
        argv: Command-line arguments (defaults to ``sys.argv[1:]``).

    Returns:
        The process exit code (0 on success, 1 on a fatal error).
    """
    args = _parse_args(argv)

    try:
        functions = load_function_definitions(args.functions_definition)
        prompts = load_test_prompts(args.input)
    except ProjectIOError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    try:
        llm = Small_LLM_Model(args.model)
    except Exception as exc:  # any failure here is fatal and must be reported clearly
        print(f"Error: could not load model '{args.model}': {exc}", file=sys.stderr)
        return 1

    try:
        vocab_path = llm.get_path_to_vocab_file()
        vocab = VocabIndex.build(load_vocab(vocab_path))
        special_tokens = load_special_tokens(llm.get_path_to_tokenizer_file())
        tokenizer = Tokenizer(
            load_raw_vocab(vocab_path), load_merges(llm.get_path_to_merges_file()), special_tokens=special_tokens
        )
    except VocabLoadError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    generator = ConstrainedJSONGenerator(llm, tokenizer, vocab, trace=args.trace)
    try:
        caller = FunctionCaller(generator, functions)
    except GenerationError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    results: list[FunctionCallResult] = []
    for test_prompt in prompts:
        try:
            results.append(caller.call(test_prompt))
        except GenerationError as exc:
            print(f"Warning: skipping prompt {test_prompt.prompt!r}: {exc}", file=sys.stderr)

    try:
        save_results(args.output, results)
    except ProjectIOError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    print(f"Wrote {len(results)}/{len(prompts)} function call(s) to {args.output}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
