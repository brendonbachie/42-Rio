*This project has been created as part of the 42 curriculum by bgomes-b.*

# RAG against the machine

## Description

This project is a **Retrieval-Augmented Generation (RAG)** system that answers
questions about a codebase. It ingests the vLLM repository into a searchable
lexical index, retrieves the most relevant snippets for a given question,
generates a natural-language answer grounded in those snippets using a small
local model (`Qwen/Qwen3-0.6B`), and measures retrieval quality with recall@k.

The pipeline has four stages: **indexing** (organise the corpus so it can be
searched), **retrieving** (match a question against the index and return the
top-k snippets), **augmenting** (place the retrieved snippets in the model's
context window), and **generating** (produce a grounded answer from that
context).

## Instructions

The project uses [`uv`](https://docs.astral.sh/uv/) as its package and project
manager. All commands are run through `uv`.

```bash
# Install dependencies (creates the virtual environment from uv.lock)
make install        # equivalent to: uv sync

# Build the index over data/raw/ (writes to data/processed/)
uv run python -m src index --max_chunk_size 2000

# Lint the codebase (flake8 + mypy)
make lint

# Remove caches and temporary files
make clean
```

The raw corpus (the vLLM repository) is expected under `data/raw/`, and the
index is written to `data/processed/`. Neither the corpus, the index, model
weights, nor generated outputs are committed to the repository; they are
produced at run time.

`Qwen/Qwen3-0.6B` is downloaded automatically on first use of the answer
commands and runs on CPU. The deep-learning stack plus the model weights total
a few gigabytes, so run the project from a location with enough free disk
space.

## System architecture

The pipeline is composed of single-responsibility modules that exchange
`pydantic` models:

- **`src/models.py`** — all `pydantic` data models (`MinimalSource`,
  `AnsweredQuestion`, `RagDataset`, `StudentSearchResults`,
  `StudentSearchResultsAndAnswer`, etc.). These validate the data exchanged
  between stages.
- **`src/chunking.py`** — the two chunking strategies (Python code and
  Markdown/text), each producing chunks that carry their absolute character
  span in the original file.
- **`src/tokenizer.py`** — the shared tokenizer used at both index time and
  query time.
- **`src/indexer.py`** — reads the corpus, dispatches each file to the right
  chunker, builds the BM25 index, and persists it.
- **`src/retriever.py`** — loads the persisted index and returns the top-k
  sources (optionally with their chunk texts, for answer generation).
- **`src/generator.py`** — loads `Qwen/Qwen3-0.6B` once and generates an
  answer grounded in the retrieved context.
- **`src/evaluator.py`** — computes recall@k against a ground-truth dataset
  (for local iteration only).
- **`src/io_utils.py`** — loads datasets and writes pipeline outputs.
- **`src/__main__.py`** — the Python Fire CLI wiring everything together.

The stages feed one another: `index` → `search`/`search_dataset` →
`answer`/`answer_dataset`, with `evaluate` measuring retrieval quality at any
point.

## Chunking strategy

A Python file and a Markdown page do not break apart the same way, so the
system implements **two distinct chunking strategies**:

- **Python code chunking** uses the `ast` module to split files by top-level
  functions and classes, keeping whole logical units together. Files that
  cannot be parsed (e.g. invalid syntax) fall back to text splitting, so the
  indexer never crashes. Code chunks use the larger size ceiling so a whole
  function stays in one chunk.
- **Markdown / text chunking** splits by header sections (`#` lines). Sections
  larger than the size limit are further split into overlapping windows so that
  content near a boundary still appears whole in at least one chunk. Docs use a
  smaller chunk size for finer granularity.

Every chunk records its `first_character_index` and `last_character_index` in
the **original file**, not in the chunk. This is what allows retrieved sources
to be scored against the ground truth by character-range overlap.

Chunk size is configurable via `--max_chunk_size` (default 2000). No chunk ever
exceeds this ceiling, which is required because the evaluator rejects any
retrieved source longer than its `max_context_length`.

## Retrieval method

Retrieval uses **BM25** (via `rank-bm25`), a lexical ranking function. BM25 was
chosen over plain TF-IDF for two reasons that matter for this corpus:

1. **Term-frequency saturation**: repeated identifiers (like `self`, `config`,
   `model`) stop adding score after a few occurrences, so a file does not rank
   highly just for repeating a common token.
2. **Length normalisation**: short, focused chunks are not penalised against
   long ones, which matters because code functions and doc sections vary widely
   in length.

The **tokenizer is shared** between indexing and querying (a mismatch would
prevent any match). It lowercases text, splits on non-alphanumerics, and
further splits compound identifiers: `load_lora_weights` yields the whole token
plus `load`, `lora`, `weights`. This lets a question match both paraphrased
ideas and identifiers quoted verbatim.

The index is persisted with `pickle` so retrieval loads it in milliseconds
rather than rebuilding it per query.

## Performance analysis

Measured on the public reference datasets (recall@5):

| Dataset | Recall@5 | Threshold | Status |
|---------|----------|-----------|--------|
| docs    | 80.0%    | 80%       | pass   |
| code    | 67.7%    | 50%       | pass   |

- **Indexing time**: ~11 seconds for the whole corpus (well under the 5-minute
  limit).
- **Retrieval throughput**: ~100 questions in ~4 seconds (well under the
  90-seconds-for-200-questions limit).

A diagnosis of the residual docs misses showed two main causes: a few cases
where the correct file is retrieved in the top-k but the returned chunk sits
just beside the answer's region (a chunk-boundary effect, addressable with more
overlap), and cases inside very large files (e.g. `supported_models.md`) where
the correct section is out-ranked by competing sections of the same file (a
ranking problem, addressable with re-ranking or semantic embeddings — the
semantic-embeddings bonus).

The reported recall uses a permissive character-overlap rule for local
iteration; the official recall@k is computed by the provided moulinette
(IoU 0.05), which the project never imports or calls.

## Design decisions

- **Separate chunk sizes for code and docs.** An early attempt to use a single
  small chunk size for everything raised docs recall slightly but dropped code
  recall (from 67.7% to 59.6%), because small chunks split functions apart.
  Code keeps larger chunks (whole functions), docs use smaller ones (focused
  sections). This is the single most impactful design decision.
- **Broad but text-only corpus.** Beyond `.py` and `.md`, the indexer also
  reads `.txt`, `.rst`, `CMakeLists.txt`, and `.cmake`, because ground-truth
  answers occasionally live in build files that a `.py`/`.md`-only indexer would
  never retrieve.
- **Errors raised internally, handled at the CLI boundary.** I/O functions
  raise typed exceptions (`FileNotFoundError`, `ValueError`); the CLI entry
  point catches exactly those and prints a clean message, so degenerate inputs
  (empty query, `k=0`, missing files, malformed JSON) never crash with a
  traceback, while genuine programming bugs still surface.
- **Deterministic generation.** The generator uses greedy decoding
  (`do_sample=False`) for reproducible answers, and disables the Qwen3
  "thinking" mode so reasoning tokens do not leak into the final answer.

## Challenges faced

- **Character-index correctness.** The hardest requirement was ensuring each
  chunk's character span points to the right region of the original file. This
  was validated with an assertion that `file[first:last]` equals the chunk
  text.
- **Chunk size trade-off.** Balancing code recall (wants large chunks) against
  docs recall (wants small chunks) required measuring both and splitting the
  configuration, rather than picking one global size.
- **Running the model on CPU without CUDA.** The default PyTorch build pulls in
  several gigabytes of NVIDIA dependencies. The CPU-only PyTorch wheel index was
  configured in `pyproject.toml` to keep the install small.
- **Re-indexing discipline.** The index is a frozen snapshot; changes to the
  chunker or tokenizer only take effect after re-running `index`. Forgetting
  this produced misleading, unchanged recall numbers until the index was
  rebuilt.

## Example usage

```bash
# 1. Index the corpus once
uv run python -m src index --max_chunk_size 2000

# 2. Search a single query
uv run python -m src search "How to configure the OpenAI server?" --k 5

# 3. Search a whole dataset (scope the output directory by dataset)
uv run python -m src search_dataset \
  --dataset_path data/datasets/UnansweredQuestions/dataset_docs_public.json \
  --k 10 \
  --save_directory data/output/search_results/UnansweredQuestions

# 4. Check your own recall@k (local iteration only)
uv run python -m src evaluate \
  --student_search_results_path data/output/search_results/AnsweredQuestions/dataset_docs_public.json \
  --dataset_path data/datasets/AnsweredQuestions/dataset_docs_public.json \
  --k 5

# 5. Answer a single query
uv run python -m src answer "How to configure the OpenAI server?" --k 5

# 6. Generate answers for a whole dataset
uv run python -m src answer_dataset \
  --student_search_results_path data/output/search_results/UnansweredQuestions/dataset_docs_public.json \
  --save_directory data/output/search_results_and_answer/UnansweredQuestions
```

## Resources

Classic references on the topic:

- BM25 / Okapi weighting — the lexical ranking function used for retrieval.
- Retrieval-Augmented Generation — the general technique of grounding a language
  model in retrieved evidence at answer time.
- [`rank-bm25`](https://github.com/dorianbrown/rank_bm25) — the BM25
  implementation used.
- [Hugging Face Transformers](https://huggingface.co/docs/transformers) and
  [`Qwen/Qwen3-0.6B`](https://huggingface.co/Qwen/Qwen3-0.6B) — the generation
  model and library.
- [`uv`](https://docs.astral.sh/uv/), [Python Fire](https://github.com/google/python-fire),
  and [`tqdm`](https://github.com/tqdm/tqdm) — tooling.

**Use of AI:** AI was used as a pair-programming assistant throughout the
pipeline — discussing design trade-offs (BM25 vs TF-IDF, chunking strategies,
separate chunk sizes for code and docs), drafting and reviewing code for each
stage, and diagnosing retrieval misses. All generated code was reviewed,
tested, and validated against the reference datasets before being kept, and the
design decisions are understood and owned by the author.