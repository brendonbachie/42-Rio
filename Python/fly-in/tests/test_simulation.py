"""Tests for fly_in.simulation.Simulation."""

import re
from pathlib import Path
from typing import Dict, FrozenSet

import pytest

from fly_in.exceptions import SimulationError
from fly_in.graph import Graph
from fly_in.models import Connection, Zone, ZoneType
from fly_in.parser import MapParser
from fly_in.simulation import Simulation

MAPS_DIR = Path(__file__).resolve().parent.parent / "maps"
MOVE_TOKEN = re.compile(r"^D(\d+)-(.+)$")


def test_linear_path_delivers_every_drone_in_order() -> None:
    _, graph = MapParser().parse(str(MAPS_DIR / "easy" / "01_linear_path.txt"))
    result = Simulation(graph, nb_drones=2).run()

    assert result.total_turns > 0
    delivered_lines = " ".join(result.log_lines)
    assert delivered_lines.count("goal") == 2
    assert set(result.turns_per_drone) == {1, 2}


def test_restricted_zone_takes_two_turns_and_reports_connection_then_zone() -> None:
    graph = Graph()
    graph.add_zone(Zone("start", 0, 0, is_start=True))
    graph.add_zone(Zone("tunnel", 1, 0, zone_type=ZoneType.RESTRICTED))
    graph.add_zone(Zone("goal", 2, 0, is_end=True))
    graph.add_connection(Connection("start", "tunnel"))
    graph.add_connection(Connection("tunnel", "goal"))

    result = Simulation(graph, nb_drones=1).run()

    assert result.log_lines[0] == "D1-start-tunnel"
    assert result.log_lines[1] == "D1-tunnel"
    assert result.log_lines[2] == "D1-goal"
    assert result.total_turns == 3


def test_zone_capacity_is_never_exceeded_on_any_map() -> None:
    for map_path in MAPS_DIR.glob("*/*.txt"):
        nb_drones, graph = MapParser().parse(str(map_path))
        try:
            result = Simulation(graph, nb_drones, path_pool_size=8, max_turns=1000).run()
        except SimulationError:
            continue  # The optional challenger map may be left unsolved.

        for snapshot in result.snapshots:
            for zone_name, residents in snapshot.zone_residents.items():
                zone = graph.zones[zone_name]
                if zone.is_start or zone.is_end:
                    continue
                assert len(residents) <= zone.max_drones, (
                    f"{map_path}: zone '{zone_name}' had {len(residents)} "
                    f"drones (max {zone.max_drones}) at turn {snapshot.turn}"
                )


def test_connection_capacity_is_never_exceeded_on_any_map() -> None:
    for map_path in MAPS_DIR.glob("*/*.txt"):
        nb_drones, graph = MapParser().parse(str(map_path))
        try:
            result = Simulation(graph, nb_drones, path_pool_size=8, max_turns=1000).run()
        except SimulationError:
            continue

        for snapshot in result.snapshots:
            usage: Dict[FrozenSet[str], int] = {}
            for move in snapshot.moves:
                match = MOVE_TOKEN.match(move)
                assert match is not None, f"malformed move token: {move!r}"
                destination = match.group(2)
                zones_in_token = destination.split("-")
                if len(zones_in_token) == 2:
                    key = frozenset(zones_in_token)
                    connection = graph.connections.get(key)
                    if connection is not None:
                        usage[key] = usage.get(key, 0) + 1

            for key, count in usage.items():
                connection = graph.connections[key]
                assert count <= connection.max_link_capacity, (
                    f"{map_path}: connection {sorted(key)} used by {count} "
                    f"drones at turn {snapshot.turn} (max "
                    f"{connection.max_link_capacity})"
                )


def test_output_format_matches_subject_grammar() -> None:
    _, graph = MapParser().parse(str(MAPS_DIR / "easy" / "02_simple_fork.txt"))
    result = Simulation(graph, nb_drones=4).run()

    for line in result.log_lines:
        if not line:
            continue
        for token in line.split(" "):
            assert MOVE_TOKEN.match(token), f"unexpected token format: {token!r}"


def test_missing_path_raises_simulation_error() -> None:
    graph = Graph()
    graph.add_zone(Zone("start", 0, 0, is_start=True))
    graph.add_zone(Zone("goal", 1, 0, is_end=True))
    # No connection between start and goal.

    with pytest.raises(SimulationError):
        Simulation(graph, nb_drones=1).run()
