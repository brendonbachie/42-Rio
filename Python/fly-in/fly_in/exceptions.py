"""Custom exceptions used across the Fly-In project."""


class FlyInError(Exception):
    """Base class for every error raised by this project."""


class MapParseError(FlyInError):
    """Raised when the input map file is malformed.

    Attributes:
        line_number: The 1-based line number where the error was found.
        reason: A human-readable description of what went wrong.
    """

    def __init__(self, line_number: int, reason: str) -> None:
        self.line_number = line_number
        self.reason = reason
        super().__init__(f"line {line_number}: {reason}")


class GraphError(FlyInError):
    """Raised when a graph operation cannot be completed."""


class SimulationError(FlyInError):
    """Raised when the simulation cannot make progress or is misconfigured."""
