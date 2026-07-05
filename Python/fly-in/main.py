"""Command-line entry point for the Fly-In drone routing simulation."""

from __future__ import annotations

import argparse
import sys
from typing import List, Optional

from fly_in.exceptions import FlyInError
from fly_in.parser import MapParser
from fly_in.simulation import Simulation, find_best_simulation
from fly_in.visualization.gui_view import GuiView
from fly_in.visualization.terminal_view import TerminalView


def build_arg_parser() -> argparse.ArgumentParser:
    """Build the CLI argument parser."""
    parser = argparse.ArgumentParser(
        prog="fly-in",
        description=(
            "Route a fleet of drones from a start hub to an end hub across a "
            "network of zones, in the fewest possible simulation turns."
        ),
    )
    parser.add_argument(
        "map_file",
        nargs="?",
        default="maps/easy/01_linear_path.txt",
        help="Path to the map file (default: %(default)s).",
    )
    parser.add_argument(
        "--gui",
        action="store_true",
        help="Also open a graphical (tkinter) replay window after the run.",
    )
    parser.add_argument(
        "--no-terminal",
        action="store_true",
        help="Skip the colored terminal output (useful together with --gui).",
    )
    parser.add_argument(
        "--path-pool-size",
        type=int,
        default=None,
        help=(
            "Fix the number of alternative routes drones are spread across. "
            "If omitted, a small range is tried automatically and the run "
            "with the fewest total turns is kept."
        ),
    )
    parser.add_argument(
        "--max-turns",
        type=int,
        default=5000,
        help="Safety limit on simulation turns before aborting (default: %(default)s).",
    )
    return parser


def main(argv: Optional[List[str]] = None) -> int:
    """Parse arguments, run the simulation and render its output.

    Returns:
        The process exit code (0 on success, 1 on any Fly-In error).
    """
    args = build_arg_parser().parse_args(argv)

    try:
        nb_drones, graph = MapParser().parse(args.map_file)
        if args.path_pool_size is not None:
            result = Simulation(
                graph,
                nb_drones,
                path_pool_size=args.path_pool_size,
                max_turns=args.max_turns,
            ).run()
        else:
            result = find_best_simulation(graph, nb_drones, max_turns=args.max_turns)
    except FlyInError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    except OSError as error:
        print(f"error: could not read map file: {error}", file=sys.stderr)
        return 1

    if not args.no_terminal:
        TerminalView(graph).render(result)

    if args.gui:
        GuiView(graph, result).run()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
