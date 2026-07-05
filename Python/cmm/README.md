*This project has been created as part of the 42 curriculum by bgomes-b.*

# call me maybe

## Description

`call me maybe` teaches a small, unreliable language model to "speak the
language of computers". Given a natural-language request such as *"What is
the sum of 2 and 3?"* and a catalog of available functions, the program does
**not** answer the question in prose. Instead it outputs a structured
function call:

```json
{"prompt": "What is the sum of 2 and 3?", "name": "fn_add_numbers", "parameters": {"a": 2.0, "b": 3.0}}
```

The twist: small models (here, `Qwen/Qwen3-0.6B`, ~500M-0.6B parameters) are
notoriously bad at spontaneously producing valid JSON — prompting alone gets
you maybe 30% reliability. This project instead uses **constrained
decoding**: at every generation step, the raw logits produced by the model
are masked down to only the tokens that keep the output both syntactically
valid JSON *and* compliant with the function's schema. The model never gets
the chance to produce anything invalid, which is what makes 100% valid,
schema-compliant JSON possible even with a tiny model.

## Instructions

### Requirements

- Python 3.10+
- [`uv`](https://docs.astral.sh/uv/) for dependency management
- The `llm_sdk` package, expected at the project root (already vendored in
  this repository, see `llm_sdk/`)

### Install

```sh
make install
# equivalent to: uv sync
```

The first run will download the `Qwen/Qwen3-0.6B` weights from the Hugging
Face Hub (a few hundred MB) unless they are already cached locally.

### Run

```sh
make run
# equivalent to: uv run python -m src
```

By default this reads `data/input/functions_definition.json` and
`data/input/function_calling_tests.json`, and writes
`data/output/function_calling_results.json`. All three paths, plus the model
name, can be overridden:

```sh
uv run python -m src \
  --functions_definition data/input/functions_definition.json \
  --input data/input/function_calling_tests.json \
  --output data/output/function_calling_results.json \
  --model Qwen/Qwen3-0.6B
```

### Debug / lint / test / clean

```sh
make debug        # run under pdb
make lint         # flake8 + mypy
make lint-strict  # flake8 + mypy --strict
make test         # pytest (unit tests, not graded, kept for our own confidence)
make clean        # remove __pycache__, .mypy_cache, .pytest_cache, data/output
```

## Resources

- [OpenAI function calling guide](https://platform.openai.com/docs/guides/function-calling) — the general concept this project reimplements from scratch.
- [Hugging Face — Guided/constrained generation concepts](https://huggingface.co/docs/transformers/main/en/generation_strategies) — background on generation loops and logits processors (we do not use the `LogitsProcessor` API itself, everything is hand-rolled).
- [GPT-2 byte-level BPE (`bytes_to_unicode`/`encoder.py`)](https://github.com/openai/gpt-2/blob/master/src/encoder.py) — the exact byte<->unicode mapping and merge-loop reference our own encoder/decoder (`src/tokenizer.py`, `src/tokenizer_vocab.py`) reimplements from scratch, without calling the SDK's `encode`/`decode` (bonus feature, see "Bonus features").
- [Hugging Face — Byte-Pair Encoding tokenization](https://huggingface.co/learn/nlp-course/chapter6/5) — walkthrough of the BPE merge algorithm used to design `src/tokenizer.py::_apply_merges`.
- [JSON grammar (json.org)](https://www.json.org/json-en.html) — reference for the number/string grammars implemented in `src/constrained_decoder.py`.
- [Pydantic v2 documentation](https://docs.pydantic.dev/latest/) — used for all schema validation (`src/models.py`).
- [uv documentation](https://docs.astral.sh/uv/) — dependency management and the `[tool.uv.sources]` local path dependency used to vendor `llm_sdk`.

### How AI was used

- **Architecture discussion**: an AI assistant was used as a sounding board
  to compare candidate designs for constrained decoding (e.g. whether to
  build a generic recursive-descent JSON grammar vs. a fixed skeleton with
  typed "holes"). The fixed-skeleton design was chosen deliberately because
  the output schema is always `{"name": ..., "parameters": {...}}` with a
  parameter set known in advance once the function is picked — a full
  general-purpose grammar would have been unnecessary complexity for this
  problem.
- **Boilerplate generation**: an AI assistant helped scaffold repetitive
  parts (pydantic models, argparse wiring, docstrings) which were then read,
  understood, and adjusted line by line.
- **Everything committed here was read, traced through by hand on paper
  examples (e.g. tracing `_consume` step-by-step for `"a": 42` and for a
  string value), and tested** before being kept. No code was copy-pasted
  without understanding what it does — see "Design decisions" below for the
  reasoning behind each non-obvious choice.

## Algorithm explanation — constrained decoding

The core idea: **never let the model choose between something valid and
something invalid.** Concretely, the desired output is always shaped like:

```json
{"name": "<one of the known function names>", "parameters": {"<param>": <value>, ...}}
```

This shape has exactly two kinds of content:

1. **Deterministic skeleton** — `{`, `"name"`, `:`, `,`, `"parameters"`,
   quotes, braces, parameter keys. Since a function's parameter list (name,
   order, types) is fully known once the function is chosen, *none* of this
   requires a decision. It is injected directly via `force_literal`, which
   simply tokenizes the literal text and appends it — no call to the model
   at all. This alone rules out the two most common failure modes of naive
   prompting: missing/extra keys and broken punctuation.

2. **Model-decided holes** — the function name, and each parameter's value.
   These are generated **token by token**:
   - At each step, `get_logits_from_input_ids` is called to get the raw
     logits for the next token.
   - A grammar function computes the *set of legal next tokens* given the
     text generated so far for this hole (e.g. "the value generated so far
     is `12`; for a `number`, legal continuations are more digits or a
     single `.`").
   - Once the value is already a complete, valid value (e.g. `12` is a
     complete number), a special extra candidate is added: the first token
     of whatever deterministic text must follow (e.g. `, "b": ` or `}`).
     This is the "stop" option.
   - The next token is the `argmax` of the logits **restricted to the legal
     candidate set** (all other logits are effectively treated as `-inf` by
     simply never considering them). Because both "keep extending the
     value" and "stop now" are real, scored candidates, **the model itself
     decides how long a value should be** — this is not a heuristic length
     cutoff.
   - If, at any step, there is exactly **one** legal candidate, the model is
     not even queried: the token is emitted directly. This happens
     constantly while spelling out a function name once its first
     diverging character has been chosen (e.g. after committing to `fn_g`,
     if `fn_greet` is the only function starting with that prefix, every
     remaining character of `greet` is forced without a single extra
     forward pass).

Grammars implemented in `src/constrained_decoder.py`:

| Type      | Legal partial value                              | Legal complete value                  |
|-----------|---------------------------------------------------|----------------------------------------|
| `number`  | optional `-`, digits, optional `.` + digits        | `-?(0|[1-9][0-9]*)(\.[0-9]+)?`         |
| `integer` | optional `-`, digits (no `.`)                      | `-?(0|[1-9][0-9]*)`                    |
| `string`  | any token without a raw `"` , `\` or control char  | any point (including the empty string) |
| `boolean` | prefix of `"true"` or `"false"`                    | exactly `"true"` or `"false"`          |
| enum (function name) | prefix of at least one candidate name  | an exact candidate name                |

Every one of these grammars guarantees, by construction, that the decoder
can never produce a token that breaks JSON syntax or the schema — so the
resulting string is always 100% parseable and always matches the schema
in `functions_definition.json`.

In fact, the implementation goes one step further: it never even
re-parses generated text. `generate_choice`/`generate_value` return
already-typed Python values (`str`/`float`/`int`/`bool`) as they are
produced, and `FunctionCaller` assembles them directly into a
`FunctionCallResult`. There is no "generate text, then `json.loads` it and
hope" step anywhere in the pipeline.

## Design decisions

- **No KV-cache, so minimize forward passes.** `get_logits_from_input_ids`
  recomputes the full forward pass from scratch every call (the SDK exposes
  no cache). Two choices follow directly from this constraint:
  1. Deterministic skeleton text is injected without ever calling the model.
  2. Whenever the grammar narrows the legal token set down to exactly one
     candidate (very common while spelling out function names once
     unambiguous), that token is emitted without a model call.
- **`numpy` for the argmax.** Value grammars such as "any safe string token"
  can have candidate sets in the tens of thousands. `numpy` fancy indexing
  (`logits_array[candidate_ids].argmax()`) keeps this fast; a pure-Python
  loop over that many candidates at every token would dominate runtime.
- **Precomputed vocabulary views (`VocabIndex`).** Token categories that
  don't depend on generation state (which tokens are pure digits, which
  tokens are safe to use inside a JSON string, `.`/`-` token ids) are
  computed once at startup in a single O(vocab size) pass, instead of being
  recomputed at every generation step.
- **The "stop" token is a single lookahead token, not a full prefix search.**
  Once a value is complete, the only extra candidate offered is the first
  token of the literal that must follow. This is a deliberate simplification
  over searching all tokenizations of that literal: because *we* choose the
  literal text (e.g. always starting a trailing separator with `,` or `}`),
  it tokenizes predictably, and this keeps the stopping decision O(1)
  instead of O(len(literal)).
- **Parameter order follows `functions_definition.json` insertion order.**
  Pydantic's `dict` preserves insertion order, so `parameters.items()` is
  used directly as the canonical argument order in both the prompt shown to
  the model and the generated skeleton.
- **`pydantic` for every data shape**, per the project's requirement, with
  `extra="ignore"` on inputs (tolerate unknown keys from evolving fixtures)
  and `extra="forbid"` on the output model (`FunctionCallResult`) to
  guarantee the output never leaks extra keys.
- **Numeric values are always emitted as the type declared in the schema**
  (`number` → Python `float`, `integer` → Python `int`), matching the
  project's example output (`"a": 2.0` for a `number` parameter, even though
  the user wrote `"2"`).

## Performance analysis

- **Correctness/reliability**: 100% valid, schema-compliant JSON by
  construction — this is not measured empirically, it is guaranteed by the
  grammar (every legal token keeps the partial output a valid JSON prefix
  matching the schema; the loop cannot terminate on an incomplete value).
- **Accuracy** (picking the right function and the right argument values) is
  bounded by the underlying model's language understanding, not by the
  decoding mechanism — constrained decoding cannot fix a wrong semantic
  choice, only guarantee that whatever choice is made is syntactically
  valid. This was validated by running the full `data/input/` fixtures
  end-to-end and manually inspecting `data/output/function_calling_results.json`
  (see "Testing strategy").
- **Speed**: the dominant cost is the number of calls to
  `get_logits_from_input_ids`, since each one is a full, uncached forward
  pass. The single-candidate shortcut and the literal-forcing skeleton
  remove the large majority of what would otherwise be "wasted" forward
  passes on deterministic punctuation, keeping the number of real model
  calls roughly proportional to the number of *characters that are actually
  ambiguous* (a handful per function name, a handful per numeric/string
  value) rather than to the length of the full JSON output.

## Challenges faced

- **The SDK has no KV-cache.** Every `get_logits_from_input_ids` call
  reprocesses the entire sequence, so naively calling it once per output
  character (including all the punctuation) would be wasteful. Solved by
  only calling the model for genuinely ambiguous decisions (see "Design
  decisions").
- **Byte-level BPE tokens don't map 1:1 to characters.** A single token can
  contain a leading-space marker, several characters, or a partial
  multi-byte UTF-8 sequence. `src/tokenizer_vocab.py` reimplements the
  standard GPT-2 byte<->unicode mapping to decode `vocab.json` into real
  text once at startup, so that all later grammar checks operate on actual
  characters instead of the SDK's internal representation.
- **Ambiguous function-name prefixes.** If one function name were a prefix
  of another (e.g. `fn_add` and `fn_add_numbers`), the grammar must allow
  *both* "keep spelling the longer name" and "stop here" at the point where
  the short name is already complete. This is handled uniformly by the same
  stop-token mechanism used for values, rather than as a special case.
- **Deciding when a numeric or string value is "done".** There is no
  explicit end-of-value token in JSON. This is resolved by only offering the
  "stop" option (the first token of the mandatory following literal) once
  the value already parses as complete, letting the model's own preference
  between "continue" and "stop" (both scored by real logits) decide the
  length.
- **Constrained decoding cannot fix a degenerate model, only invalid JSON.**
  Running the pipeline against the real model on the hardest prompt
  (`"Replace all numbers in ... with NUMBERS"`, which asks for a regex
  argument) surfaced a real small-model failure mode: the model got stuck
  looping the fragment `"([0-9]+) | "` for the entire `string` value, which
  is *syntactically* a perfectly valid JSON string, so the grammar alone had
  no reason to stop it before the token budget. Since this is a semantic
  failure, not a syntax one, it needed two semantic-level fixes:
  1. `_consume` now accepts an optional `repair` hook
     (`_find_repetition_trim_point` in `src/constrained_decoder.py`),
     applied only to `string` values, which detects a 3-20 character chunk
     repeated 3 times in a row and trims the value back down to a *single*
     clean copy of that chunk (not just wherever the repetition happened to
     be first noticed - the tail-based detection can see a phase-shifted
     rotation of the true repeating unit, so the trim point is found by
     extending the match backward character-by-character to the run's real
     start). It also re-derives the token ids for the trimmed value instead
     of keeping the discarded repetition in the model's context for later
     steps. This is deliberately scoped to strings only (not numbers or the
     function name enum), because short repeats are legitimate there (e.g.
     `111000`).
  2. The system prompt (`src/prompt_builder.py`) now explicitly asks for "one
     concise, general pattern" when an argument is a pattern/expression,
     instead of leaving the model to improvise a format. This targets the
     root cause (the model's confused approach to the task) rather than
     only cleaning up its symptom.
- **A from-scratch tokenizer that round-trips text correctly can still
  silently degrade accuracy.** Once `src/tokenizer.py` (the "recoded
  tokenizer" bonus) replaced the SDK's `encode()` for the initial prompt,
  two end-to-end regressions appeared: `"Greet shrek"`/`"Greet john"`
  started resolving to the wrong function entirely. The generated JSON was
  still 100% valid and `Tokenizer.decode(Tokenizer.encode(text)) == text`
  held for every sample tested - the bug was invisible to round-trip
  testing, because *which* tokens represent a string matters to the model
  even when the *decoded text* is identical. Diagnosed by encoding the
  exact same prompt with both our tokenizer and the SDK's real tokenizer
  side by side and diffing the id sequences, three real gaps were found and
  fixed, each verified this way before moving to the next:
  1. Chat markers (`<|im_start|>`/`<|im_end|>`) aren't in `vocab.json` at
     all - they live in `tokenizer.json`'s `added_tokens`, with ids outside
     the base vocabulary (`src/tokenizer_vocab.py::load_special_tokens`,
     via the SDK's public `get_path_to_tokenizer_file()`).
  2. English contractions (`'s`, `'t`, `'re`, ...) must be pre-tokenized as
     one atomic unit *only when they directly follow a letter* - naively
     matching them anywhere let the opening quote of `'dog'` get mistaken
     for the contraction `'d`, corrupting the token stream for an otherwise
     unrelated word.
  3. Underscores must stay grouped with adjacent letters (`fn_greet` as one
     pre-token, not `fn` + `_` + `greet`), and a run of consecutive spaces
     must be grouped as one pre-token (all but the trailing space) instead
     of many single-space pre-tokens, so real vocabulary merges (e.g. for
     multi-space JSON indentation) can actually apply - BPE only ever
     merges *within* a pre-token, so wrong word boundaries silently block
     merges no amount of correct merge-priority logic can recover.
  After these fixes, encoding all 11 provided test prompts with our own
  tokenizer produces byte-for-byte identical token ids to the SDK's real
  tokenizer (verified directly, not just inferred from correct output).

## Testing strategy

Two complementary layers:

1. **Unit/integration tests with a fake, deterministic backend**
   (`tests/`, run with `make test`). `tests/fakes.py` implements a
   character-level fake model that mimics the SDK's structural interface
   (`encode`, `get_logits_from_input_ids`) without downloading or running
   any real model. This validates the *grammar engine itself*
   (`src/constrained_decoder.py`, `src/function_caller.py`,
   `src/tokenizer_vocab.py`, `src/io_utils.py`) deterministically and
   quickly: valid JSON shape, correct Python types per schema, minimum
   length enforcement (e.g. a number can never stop before at least one
   digit, even if a backend strongly prefers to), correct error handling
   for missing/invalid input files. These tests are not part of the graded
   submission (mentioned only for our own development confidence, per the
   project's "Additional Guidelines").
2. **End-to-end runs against the real model.** `make run` was executed
   against the provided `data/input/function_calling_tests.json` and
   `data/input/functions_definition.json`, and the resulting
   `data/output/function_calling_results.json` was manually checked against
   each prompt: correct function picked, correct argument values, and valid
   types. Edge cases explicitly exercised: prompts requiring negative-looking
   phrasing, multi-parameter functions (`fn_substitute_string_with_regex`),
   and functions with a single string parameter (`fn_reverse_string`). On
   the provided fixture, 11/11 prompts resolved to a correct function and
   correctly-typed arguments. This run is exactly what caught the
   degenerate-repetition failure mode described in "Challenges faced" —
   a class of bug that only shows up against the real model, never against
   the deterministic fake backend, which is why both layers of testing are
   kept.

## Example usage

```sh
$ make install
$ make run
Wrote 11/11 function call(s) to data/output/function_calling_results.json
```

```sh
$ cat data/output/function_calling_results.json
[
  {
    "prompt": "What is the sum of 2 and 3?",
    "name": "fn_add_numbers",
    "parameters": {"a": 2.0, "b": 3.0}
  },
  {
    "prompt": "Reverse the string 'hello'",
    "name": "fn_reverse_string",
    "parameters": {"s": "hello"}
  }
]
```

Custom input/output paths and a different model:

```sh
uv run python -m src \
  --functions_definition path/to/custom_functions.json \
  --input path/to/custom_prompts.json \
  --output path/to/custom_results.json \
  --model Qwen/Qwen3-0.6B
```

Watching the generation process step by step (see "Bonus features" below):

```sh
uv run python -m src --trace
```

## Bonus features

All of the following are implemented and working (not just described):

- **Recoded tokenizer, no SDK `encode`/`decode`** (`src/tokenizer.py`,
  `src/tokenizer_vocab.py::load_raw_vocab`/`load_merges`). The main code
  path never calls `Small_LLM_Model.encode`/`.decode`. Instead,
  `ConstrainedJSONGenerator` is given a from-scratch `Tokenizer`, built only
  from `get_path_to_vocab_file()` and `get_path_to_merges_file()`. It
  reimplements byte-level BPE end to end: word splitting with the
  leading-space convention, byte→unicode mapping, iterative highest-priority
  merge application, and vocabulary lookup — see `Tokenizer.encode`. A
  public `Tokenizer.decode` is included too. `tests/test_tokenizer.py`
  verifies encode/decode round-trip fidelity on realistic text (including
  multi-space JSON indentation and newlines) and the merge-priority logic
  with a small hand-built vocabulary. This one piece of work covers three
  items from the bonus list at once: recoding the tokenizer, a public
  encode/decode implementation, and demonstrating how encoding integrates
  with constrained decoding (every forced literal and every generated value
  is tokenized through it).
- **Advanced error recovery: per-entry validation** (`src/io_utils.py::_load_json_array`).
  A single malformed entry in `functions_definition.json` or
  `function_calling_tests.json` (e.g. a typo'd field) no longer aborts the
  whole file — it is validated individually, skipped with a warning on
  stderr, and the rest of the file is still processed. See
  `tests/test_io_utils.py::test_load_function_definitions_skips_only_the_malformed_entries`.
- **Visualization of the generation process** (`--trace` flag,
  `ConstrainedJSONGenerator._log`). When passed, every constrained-decoding
  step is printed to stderr: whether a token was *forced* (only one
  grammar-legal candidate, no model call needed), *chosen by the model*
  (with the candidate count), or a *repair*/*stop* event — making the
  "model decides the value, not a heuristic" claim in "Algorithm
  explanation" directly observable. See
  `tests/test_constrained_decoder.py::test_trace_mode_prints_steps_to_stderr`.
- **Support for multiple LLM models** (`--model` flag, default
  `Qwen/Qwen3-0.6B`, see `src/__main__.py`). Nothing in `src/` is
  Qwen-specific: `VocabIndex` and `Tokenizer` are built purely from whatever
  `vocab.json`/`merges.txt` the given model exposes, so any model using the
  same byte-level BPE scheme (the GPT-2 family, Qwen2/3, etc.) works without
  code changes.
- **Comprehensive test suite** (`tests/`, 33 tests, `make test`). Covers the
  grammar engine (every value type, minimum-length enforcement, repetition
  repair, error paths), the tokenizer (round-trip fidelity, merge priority),
  I/O error handling (missing/invalid files, partial schema failures), and
  end-to-end function calling — all against a deterministic fake backend
  (`tests/fakes.py`), so the suite runs in well under a second with no
  model download required.
