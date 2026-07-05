"""Colored terminal visualization of a simulation run."""

from __future__ import annotations

from typing import List, Optional

from ..graph import Graph
from ..simulation import SimulationResult
from .colors import colorize

try:  # pragma: no cover - best effort ANSI support on older Windows consoles.
    import colorama

    colorama.just_fix_windows_console()
except ImportError:  # pragma: no cover
    pass


class TerminalView:
    """Prints a simulation run turn-by-turn with colored drone movements."""

    def __init__(self, graph: Graph) -> None:
        self.graph = graph

    def render(self, result: SimulationResult) -> None:
        """Print the full simulation log with a short summary at the end."""
        nb_drones = len(result.turns_per_drone)
        print(
            f"Simulation started: {nb_drones} drone(s), "
            f"start='{self.graph.start_zone}', end='{self.graph.end_zone}'\n"
        )
        for snapshot in result.snapshots:
            print(self._format_turn(snapshot.turn, snapshot.moves))

        print()
        print(
            f"All drones delivered in {result.total_turns} turns "
            f"(average {result.average_turns_per_drone:.2f} turns/drone)."
        )

    def _format_turn(self, turn: int, moves: List[str]) -> str:
        if not moves:
            return f"Turn {turn:>3}: (no movement)"
        tokens = [self._colorize_move(move) for move in moves]
        return f"Turn {turn:>3}: " + " ".join(tokens)

    def _colorize_move(self, move: str) -> str:
        _, _, destination = move.partition("-")
        target_zone_name = destination.rsplit("-", 1)[-1]
        zone = self.graph.zones.get(target_zone_name)
        color: Optional[str] = zone.color if zone else None
        return colorize(move, color)
