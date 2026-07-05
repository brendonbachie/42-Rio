"""Robust, error-tolerant JSON I/O helpers for the function-calling pipeline."""

import json
import sys
from pathlib import Path
from typing import TypeVar

from pydantic import BaseModel, ValidationError

from src.models import FunctionCallResult, FunctionDefinition, TestPrompt

ModelT = TypeVar("ModelT", bound=BaseModel)


class ProjectIOError(RuntimeError):
    """Raised for any recoverable input/output failure with a clear message."""


def _load_json_array(path: str, label: str, model: type[ModelT]) -> list[ModelT]:
    """Read, parse and schema-validate a JSON array file.

    Each array element is validated individually: a single malformed entry
    (e.g. a typo'd field in one function definition) is skipped with a
    warning on stderr instead of aborting the entire file - the rest of a
    large, mostly-valid file should still be usable.

    Args:
        path: Path to the JSON file.
        label: Human-readable name of the file, used in error messages.
        model: The pydantic model each array element must validate against.

    Returns:
        The validated list of model instances (possibly shorter than the
        input array, if some entries were skipped).

    Raises:
        ProjectIOError: If the file is missing, unreadable, not valid JSON,
            or not a top-level array. Individual element schema errors are
            warnings, not fatal errors.
    """
    file_path = Path(path)
    try:
        raw_text = file_path.read_text(encoding="utf-8")
    except OSError as exc:
        raise ProjectIOError(f"Could not read {label} at '{path}': {exc}") from exc

    try:
        data = json.loads(raw_text)
    except json.JSONDecodeError as exc:
        raise ProjectIOError(f"{label} at '{path}' is not valid JSON: {exc}") from exc

    if not isinstance(data, list):
        raise ProjectIOError(f"{label} at '{path}' must contain a JSON array at the top level.")

    validated: list[ModelT] = []
    for index, item in enumerate(data):
        try:
            validated.append(model.model_validate(item))
        except ValidationError as exc:
            print(f"Warning: skipping entry {index} of {label} at '{path}': {exc}", file=sys.stderr)
    return validated


def load_function_definitions(path: str) -> list[FunctionDefinition]:
    """Load and validate the function definitions file.

    Args:
        path: Path to ``functions_definition.json``.

    Returns:
        The list of validated function definitions.

    Raises:
        ProjectIOError: On missing/invalid file or schema validation errors.
    """
    return _load_json_array(path, "functions definition file", FunctionDefinition)


def load_test_prompts(path: str) -> list[TestPrompt]:
    """Load and validate the natural-language test prompts file.

    Args:
        path: Path to ``function_calling_tests.json``.

    Returns:
        The list of validated test prompts.

    Raises:
        ProjectIOError: On missing/invalid file or schema validation errors.
    """
    return _load_json_array(path, "input prompts file", TestPrompt)


def save_results(path: str, results: list[FunctionCallResult]) -> None:
    """Write the resolved function calls to disk as a JSON array.

    Args:
        path: Destination path for ``function_calling_results.json``.
        results: The function call results to serialize.

    Raises:
        ProjectIOError: If the destination cannot be created or written.
    """
    file_path = Path(path)
    try:
        file_path.parent.mkdir(parents=True, exist_ok=True)
        payload = [result.model_dump(mode="json") for result in results]
        with file_path.open("w", encoding="utf-8") as handle:
            json.dump(payload, handle, indent=2, ensure_ascii=False)
            handle.write("\n")
    except OSError as exc:
        raise ProjectIOError(f"Could not write output file at '{path}': {exc}") from exc
