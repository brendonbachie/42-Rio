"""Ties the prompt builder and the constrained decoder to the function schema.

The JSON output is never generated as free text and re-parsed: the function
name and every argument value are produced incrementally as typed Python
values by :class:`~src.constrained_decoder.ConstrainedJSONGenerator`, and
assembled here directly into a :class:`~src.models.FunctionCallResult`. The
JSON skeleton (keys, quotes, braces, commas) is injected deterministically
between generation steps, so it can never be malformed.
"""

from __future__ import annotations

from src.constrained_decoder import ConstrainedJSONGenerator, GenerationError
from src.models import FunctionCallResult, FunctionDefinition, TestPrompt
from src.prompt_builder import build_prompt


class FunctionCaller:
    """Resolves natural-language prompts into schema-valid function calls."""

    def __init__(self, generator: ConstrainedJSONGenerator, functions: list[FunctionDefinition]) -> None:
        """Initialize the caller with a set of callable function definitions.

        Args:
            generator: The constrained JSON generator wrapping the LLM.
            functions: The available function definitions to choose from.

        Raises:
            GenerationError: If no function definitions are available.
        """
        if not functions:
            raise GenerationError("No function definitions were provided; cannot pick a function to call.")
        self._generator = generator
        self._functions = {fn.name: fn for fn in functions}
        self._function_names = list(self._functions.keys())

    def call(self, test_prompt: TestPrompt) -> FunctionCallResult:
        """Resolve a single natural-language prompt into a function call.

        Args:
            test_prompt: The natural-language request to translate.

        Returns:
            The resolved, schema-compliant function call.

        Raises:
            GenerationError: If the model cannot produce a grammar-legal
                function call within the configured token budget.
        """
        gen = self._generator
        prompt_text = build_prompt(list(self._functions.values()), test_prompt.prompt)

        input_ids = gen.force_literal([], prompt_text)
        input_ids = gen.force_literal(input_ids, '{"name": "')

        name, input_ids = gen.generate_choice(input_ids, self._function_names, '", "parameters": {')
        function_def = self._functions[name]

        parameters: dict[str, object] = {}
        param_items = list(function_def.parameters.items())
        if not param_items:
            gen.force_literal(input_ids, "}}")

        for index, (param_name, schema) in enumerate(param_items):
            is_last = index == len(param_items) - 1
            # The closing braces are folded into the *last* parameter's own
            # literal_after (instead of forced separately once the loop
            # ends) so that its value grammar always has a non-empty "stop"
            # anchor to offer the model - a number/integer grammar never
            # naturally runs out of legal continuations on its own.
            separator = "" if is_last else ", "
            closing = "}}" if is_last else ""

            input_ids = gen.force_literal(input_ids, f'"{param_name}": ')
            if schema.type == "string":
                input_ids = gen.force_literal(input_ids, '"')
                literal_after = f'"{separator}{closing}'
            else:
                literal_after = f"{separator}{closing}"
            value, input_ids = gen.generate_value(input_ids, schema.type, literal_after)
            parameters[param_name] = value

        return FunctionCallResult(prompt=test_prompt.prompt, name=name, parameters=parameters)
