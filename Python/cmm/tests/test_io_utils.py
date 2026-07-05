"""Tests for the robust JSON I/O helpers."""

import json
from pathlib import Path

import pytest

from src.io_utils import ProjectIOError, load_function_definitions, load_test_prompts, save_results
from src.models import FunctionCallResult

_VALID_FUNCTIONS = [
    {
        "name": "fn_add_numbers",
        "description": "Add two numbers together.",
        "parameters": {"a": {"type": "number"}, "b": {"type": "number"}},
        "returns": {"type": "number"},
    }
]


def test_load_function_definitions_happy_path(tmp_path: Path) -> None:
    file_path = tmp_path / "functions.json"
    file_path.write_text(json.dumps(_VALID_FUNCTIONS), encoding="utf-8")

    functions = load_function_definitions(str(file_path))

    assert len(functions) == 1
    assert functions[0].name == "fn_add_numbers"
    assert list(functions[0].parameters.keys()) == ["a", "b"]


def test_load_function_definitions_missing_file_raises(tmp_path: Path) -> None:
    with pytest.raises(ProjectIOError):
        load_function_definitions(str(tmp_path / "missing.json"))


def test_load_function_definitions_invalid_json_raises(tmp_path: Path) -> None:
    file_path = tmp_path / "functions.json"
    file_path.write_text("not json at all", encoding="utf-8")

    with pytest.raises(ProjectIOError):
        load_function_definitions(str(file_path))


def test_load_function_definitions_not_an_array_raises(tmp_path: Path) -> None:
    file_path = tmp_path / "functions.json"
    file_path.write_text(json.dumps({"not": "an array"}), encoding="utf-8")

    with pytest.raises(ProjectIOError):
        load_function_definitions(str(file_path))


def test_load_function_definitions_skips_only_the_malformed_entries(
    tmp_path: Path, capsys: pytest.CaptureFixture[str]
) -> None:
    """Advanced error recovery: one malformed entry must not take down the
    rest of an otherwise valid, larger file.
    """
    file_path = tmp_path / "functions.json"
    file_path.write_text(
        json.dumps([{"description": "missing name and parameters"}, *_VALID_FUNCTIONS]), encoding="utf-8"
    )

    functions = load_function_definitions(str(file_path))

    assert len(functions) == 1
    assert functions[0].name == "fn_add_numbers"
    assert "skipping entry 0" in capsys.readouterr().err


def test_load_function_definitions_all_malformed_returns_empty_list(tmp_path: Path) -> None:
    file_path = tmp_path / "functions.json"
    file_path.write_text(json.dumps([{"description": "missing name and parameters"}]), encoding="utf-8")

    assert load_function_definitions(str(file_path)) == []


def test_load_test_prompts_happy_path(tmp_path: Path) -> None:
    file_path = tmp_path / "prompts.json"
    file_path.write_text(json.dumps([{"prompt": "Greet shrek"}]), encoding="utf-8")

    prompts = load_test_prompts(str(file_path))

    assert prompts[0].prompt == "Greet shrek"


def test_save_results_creates_parent_directories(tmp_path: Path) -> None:
    output_path = tmp_path / "nested" / "output.json"
    results = [FunctionCallResult(prompt="Greet shrek", name="fn_greet", parameters={"name": "shrek"})]

    save_results(str(output_path), results)

    saved = json.loads(output_path.read_text(encoding="utf-8"))
    assert saved == [{"prompt": "Greet shrek", "name": "fn_greet", "parameters": {"name": "shrek"}}]
