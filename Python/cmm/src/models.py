"""Pydantic data models for function definitions, prompts and results."""

from typing import Any, Literal

from pydantic import BaseModel, ConfigDict, Field

SchemaType = Literal["number", "integer", "string", "boolean"]


class ParameterSchema(BaseModel):
    """Type schema for a single function parameter.

    Attributes:
        type: The JSON-ish type of the parameter (number, integer, string,
            boolean).
    """

    model_config = ConfigDict(extra="ignore")

    type: SchemaType


class ReturnSchema(BaseModel):
    """Type schema for a function's return value.

    Attributes:
        type: The JSON-ish type of the return value.
    """

    model_config = ConfigDict(extra="ignore")

    type: SchemaType


class FunctionDefinition(BaseModel):
    """Definition of a single callable function.

    Attributes:
        name: The unique name of the function (e.g. ``fn_add_numbers``).
        description: A natural-language description used to help the LLM
            pick the right function.
        parameters: Mapping of parameter name to its type schema. Insertion
            order is preserved and used as the canonical argument order.
        returns: Type schema of the function's return value.
    """

    model_config = ConfigDict(extra="ignore")

    name: str
    description: str
    parameters: dict[str, ParameterSchema] = Field(default_factory=dict)
    returns: ReturnSchema | None = None


class TestPrompt(BaseModel):
    """A single natural-language prompt to translate into a function call."""

    model_config = ConfigDict(extra="ignore")

    prompt: str


class FunctionCallResult(BaseModel):
    """A resolved function call produced for a given prompt.

    Attributes:
        prompt: The original natural-language request.
        name: The name of the function chosen by the LLM.
        parameters: The argument values extracted by the LLM, already cast
            to their expected Python types.
    """

    model_config = ConfigDict(extra="forbid")

    prompt: str
    name: str
    parameters: dict[str, Any]
