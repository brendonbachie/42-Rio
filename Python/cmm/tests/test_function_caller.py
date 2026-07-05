"""Integration tests: prompt -> constrained decoding -> FunctionCallResult."""

import pytest

from src.constrained_decoder import ConstrainedJSONGenerator, GenerationError, VocabIndex
from src.function_caller import FunctionCaller
from src.models import FunctionDefinition, ParameterSchema, TestPrompt
from tests.fakes import FakeLLM, build_fake_tokenizer, build_fake_vocab


def _functions() -> list[FunctionDefinition]:
    return [
        FunctionDefinition(
            name="fn_add_numbers",
            description="Add two numbers together and return their sum.",
            parameters={"a": ParameterSchema(type="number"), "b": ParameterSchema(type="number")},
        ),
        FunctionDefinition(
            name="fn_greet",
            description="Generate a greeting message for a person by name.",
            parameters={"name": ParameterSchema(type="string")},
        ),
    ]


def _caller() -> FunctionCaller:
    vocab = VocabIndex.build(build_fake_vocab())
    generator = ConstrainedJSONGenerator(FakeLLM(), build_fake_tokenizer(), vocab)
    return FunctionCaller(generator, _functions())


def test_call_returns_a_known_function_with_matching_parameter_keys() -> None:
    caller = _caller()

    result = caller.call(TestPrompt(prompt="What is the sum of 2 and 3?"))

    assert result.prompt == "What is the sum of 2 and 3?"
    assert result.name in {"fn_add_numbers", "fn_greet"}
    expected_params = {"fn_add_numbers": {"a", "b"}, "fn_greet": {"name"}}
    assert set(result.parameters.keys()) == expected_params[result.name]


def test_call_produces_correctly_typed_parameters() -> None:
    caller = _caller()

    result = caller.call(TestPrompt(prompt="Greet shrek"))

    if result.name == "fn_greet":
        assert isinstance(result.parameters["name"], str)
    else:
        assert isinstance(result.parameters["a"], float)
        assert isinstance(result.parameters["b"], float)


def test_call_raises_on_empty_function_list() -> None:
    vocab = VocabIndex.build(build_fake_vocab())
    generator = ConstrainedJSONGenerator(FakeLLM(), build_fake_tokenizer(), vocab)

    with pytest.raises(GenerationError):
        FunctionCaller(generator, [])
