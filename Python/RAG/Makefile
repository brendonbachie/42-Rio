.PHONY: install run debug clean lint lint-strict

install:
	uv sync

run:
	uv run python -m src index --max_chunk_size 2000

debug:
	uv run python -m pdb -m src index --max_chunk_size 2000

clean:
	find . -type d -name __pycache__ -exec rm -rf {} +
	rm -rf .mypy_cache

lint:
	uv run flake8 .
	uv run mypy . --warn-return-any --warn-unused-ignores \
		--ignore-missing-imports --disallow-untyped-defs --check-untyped-defs

lint-strict:
	uv run flake8 .
	uv run mypy . --strict
