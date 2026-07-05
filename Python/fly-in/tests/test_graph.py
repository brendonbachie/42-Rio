"""Tests for fly_in.graph.Graph."""

from fly_in.graph import Graph
from fly_in.models import Connection, Zone, ZoneType


def make_fork_graph() -> Graph:
    graph = Graph()
    graph.add_zone(Zone("start", 0, 0, is_start=True))
    graph.add_zone(Zone("junction", 1, 0))
    graph.add_zone(Zone("path_a", 2, 1))
    graph.add_zone(Zone("path_b", 2, -1))
    graph.add_zone(Zone("goal", 3, 0, is_end=True))
    graph.add_connection(Connection("start", "junction"))
    graph.add_connection(Connection("junction", "path_a"))
    graph.add_connection(Connection("junction", "path_b"))
    graph.add_connection(Connection("path_a", "goal"))
    graph.add_connection(Connection("path_b", "goal"))
    return graph


def test_shortest_path_finds_a_route() -> None:
    graph = make_fork_graph()
    path = graph.shortest_path("start", "goal")
    assert path is not None
    assert path[0] == "start"
    assert path[-1] == "goal"
    assert len(path) == 4


def test_shortest_path_avoids_blocked_zones() -> None:
    graph = Graph()
    graph.add_zone(Zone("start", 0, 0, is_start=True))
    graph.add_zone(Zone("wall", 1, 0, zone_type=ZoneType.BLOCKED))
    graph.add_zone(Zone("goal", 2, 0, is_end=True))
    graph.add_connection(Connection("start", "wall"))
    graph.add_connection(Connection("wall", "goal"))

    assert graph.shortest_path("start", "goal") is None


def test_shortest_path_prefers_priority_over_equal_cost_normal() -> None:
    graph = Graph()
    graph.add_zone(Zone("start", 0, 0, is_start=True))
    graph.add_zone(Zone("normal_hop", 1, 0, zone_type=ZoneType.NORMAL))
    graph.add_zone(Zone("priority_hop", 1, 1, zone_type=ZoneType.PRIORITY))
    graph.add_zone(Zone("goal", 2, 0, is_end=True))
    graph.add_connection(Connection("start", "normal_hop"))
    graph.add_connection(Connection("start", "priority_hop"))
    graph.add_connection(Connection("normal_hop", "goal"))
    graph.add_connection(Connection("priority_hop", "goal"))

    path = graph.shortest_path("start", "goal")
    assert path == ["start", "priority_hop", "goal"]


def test_k_shortest_paths_distributes_across_the_fork() -> None:
    graph = make_fork_graph()
    paths = graph.k_shortest_paths("start", "goal", 4)

    assert len(paths) == 2
    destinations = {path[2] for path in paths}
    assert destinations == {"path_a", "path_b"}


def test_k_shortest_paths_rejects_head_on_direction_conflicts() -> None:
    graph = Graph()
    graph.add_zone(Zone("start", 0, 0, is_start=True))
    graph.add_zone(Zone("gate_a", 1, 0, max_drones=1))
    graph.add_zone(Zone("gate_b", 2, 0, max_drones=1))
    graph.add_zone(Zone("goal", 3, 0, is_end=True))
    graph.add_connection(Connection("start", "gate_a"))
    graph.add_connection(Connection("start", "gate_b"))
    graph.add_connection(Connection("gate_a", "gate_b"))
    graph.add_connection(Connection("gate_a", "goal"))
    graph.add_connection(Connection("gate_b", "goal"))

    paths = graph.k_shortest_paths("start", "goal", 5)
    used_directions = set()
    for path in paths:
        for zone_a, zone_b in zip(path, path[1:]):
            used_directions.add((zone_a, zone_b))

    for zone_a, zone_b in used_directions:
        assert (zone_b, zone_a) not in used_directions
