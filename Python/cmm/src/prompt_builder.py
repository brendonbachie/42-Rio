"""Builds the chat-style prompt sent to the model before constrained decoding."""

import json

from src.models import FunctionDefinition

IM_START = "<|im_start|>"
IM_END = "<|im_end|>"

_SYSTEM_PROMPT = (
    "You are a function calling assistant. Given a user request and a list of "
    "available functions, pick the single function that best satisfies the "
    "request and infer the correct value for each of its arguments from the "
    "request. Respond only with the requested JSON, nothing else. When an "
    "argument's value is a pattern or expression (such as a regular "
    "expression), write ONE concise, general pattern that covers the case "
    "described - never repeat the same fragment multiple times."
)


def _describe_functions(functions: list[FunctionDefinition]) -> str:
    """Serialize function definitions into a compact JSON block for the prompt."""
    described = [
        {
            "name": fn.name,
            "description": fn.description,
            "parameters": {name: schema.type for name, schema in fn.parameters.items()},
        }
        for fn in functions
    ]
    return json.dumps(described, indent=2)


def build_prompt(functions: list[FunctionDefinition], user_prompt: str) -> str:
    """Build the full chat-formatted prompt text for a single user request.

    Args:
        functions: The available function definitions.
        user_prompt: The natural-language request to translate.

    Returns:
        The prompt text, ready to be tokenized and fed to the model. The
        text ends right where the assistant's turn begins, so constrained
        decoding can take over immediately.
    """
    user_turn = (
        f"Available functions:\n{_describe_functions(functions)}\n\n"
        f'User request: "{user_prompt}"\n\n'
        "Call the correct function with the correct arguments."
    )
    return (
        f"{IM_START}system\n{_SYSTEM_PROMPT}{IM_END}\n"
        f"{IM_START}user\n{user_turn}{IM_END}\n"
        f"{IM_START}assistant\n"
    )
