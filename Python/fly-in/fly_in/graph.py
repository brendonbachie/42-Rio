"""Graph representation and custom pathfinding (Dijkstra, no external libs)."""

from __future__ import annotations

import heapq
from typing import Dict, FrozenSet, List, Optional, Set, Tuple

from .exceptions import GraphError
from .models import Connection, Zone, ZoneType

Edge = Tuple[str, str]


def _edge(zone_a: str, zone_b: str) -> Edge:
    """A direction-independent key for the undirected edge between two zones."""
    return (zone_a, zone_b) if zone_a <= zone_b else (zone_b, zone_a)


class Graph:
    """A network of zones connected by bidirectional links."""

    def __init__(self) -> None:
        self.zones: Dict[str, Zone] = {}
        self.connections: Dict[FrozenSet[str], Connection] = {}
        self._adjacency: Dict[str, List[str]] = {}
        self.start_zone: Optional[str] = None
        self.end_zone: Optional[str] = None

    def add_zone(self, zone: Zone) -> None:
        """Register a zone in the graph."""
        self.zones[zone.name] = zone
        self._adjacency.setdefault(zone.name, [])
        if zone.is_start:
            self.start_zone = zone.name
        if zone.is_end:
            self.end_zone = zone.name

    def add_connection(self, connection: Connection) -> None:
        """Register a bidirectional connection between two known zones."""
        key = frozenset((connection.zone_a, connection.zone_b))
        self.connections[key] = connection
        self._adjacency.setdefault(connection.zone_a, []).append(connection.zone_b)
        self._adjacency.setdefault(connection.zone_b, []).append(connection.zone_a)

    def has_zone(self, name: str) -> bool:
        return name in self.zones

    def neighbors(self, zone_name: str) -> List[str]:
        """Zones directly reachable from ``zone_name``."""
        return self._adjacency.get(zone_name, [])

    def get_connection(self, zone_a: str, zone_b: str) -> Connection:
        """Return the connection linking two zones, regardless of order."""
        key = frozenset((zone_a, zone_b))
        connection = self.connections.get(key)
        if connection is None:
            raise GraphError(f"No connection between {zone_a!r} and {zone_b!r}")
        return connection

    def shortest_path(
        self,
        start: str,
        end: str,
        banned_edges: Optional[Set[Edge]] = None,
        banned_nodes: Optional[Set[str]] = None,
    ) -> Optional[List[str]]:
        """Find the cheapest path from ``start`` to ``end``.

        Uses Dijkstra's algorithm with each zone's ``path_weight`` as the
        cost of entering it. Blocked zones, banned edges and banned nodes
        (other than ``start``/``end`` themselves) are excluded from the
        search. Returns ``None`` if no path exists.
        """
        banned_edges = banned_edges or set()
        banned_nodes = banned_nodes or set()
        distances: Dict[str, int] = {start: 0}
        previous: Dict[str, str] = {}
        visited: Set[str] = set()
        queue: List[Tuple[int, str]] = [(0, start)]

        while queue:
            dist, current = heapq.heappop(queue)
            if current in visited:
                continue
            visited.add(current)
            if current == end:
                break

            for neighbor in self.neighbors(current):
                if neighbor in banned_nodes and neighbor != end:
                    continue
                zone = self.zones[neighbor]
                if zone.zone_type == ZoneType.BLOCKED:
                    continue
                edge = _edge(current, neighbor)
                if edge in banned_edges:
                    continue
                new_dist = dist + zone.path_weight
                if new_dist < distances.get(neighbor, float("inf")):
                    distances[neighbor] = new_dist
                    previous[neighbor] = current
                    heapq.heappush(queue, (new_dist, neighbor))

        if end not in distances:
            return None

        path = [end]
        while path[-1] != start:
            path.append(previous[path[-1]])
        path.reverse()
        return path

    def path_cost(self, path: List[str]) -> int:
        """Total pathfinding weight of entering every zone after the first."""
        return sum(self.zones[name].path_weight for name in path[1:])

    def k_shortest_paths(self, start: str, end: str, k: int) -> List[List[str]]:
        """Return up to ``k`` distinct low-cost paths from ``start`` to ``end``.

        A simplified version of Yen's algorithm: starting from the single
        cheapest path, it repeatedly branches off at every node of the
        previously accepted paths (banning only the specific edge and the
        already-visited nodes of that branch, not the whole path) to find
        genuinely different detours. This lets drones be spread across
        parallel routes even when they all share a single entry edge.

        Candidates that would traverse an already-used edge in the opposite
        direction are rejected: two drones crossing the same edge head-on
        can deadlock forever when the zones on either end only hold one
        drone at a time, so the accepted pool is kept direction-consistent.
        """
        first = self.shortest_path(start, end)
        if first is None:
            return []

        accepted: List[List[str]] = [first]
        directed_edges_used: Set[Edge] = set(zip(first, first[1:]))
        candidates: List[Tuple[int, List[str]]] = []

        while len(accepted) < k:
            previous_path = accepted[-1]
            for i in range(len(previous_path) - 1):
                spur_node = previous_path[i]
                root_path = previous_path[: i + 1]

                banned_edges: Set[Edge] = set()
                for path in accepted:
                    if len(path) > i and path[: i + 1] == root_path:
                        banned_edges.add(_edge(path[i], path[i + 1]))

                banned_nodes = set(root_path[:-1])
                spur_path = self.shortest_path(
                    spur_node, end, banned_edges, banned_nodes
                )
                if spur_path is None:
                    continue

                total_path = root_path[:-1] + spur_path
                if total_path in accepted:
                    continue
                if any(candidate == total_path for _, candidate in candidates):
                    continue
                candidates.append((self.path_cost(total_path), total_path))

            # Candidates queued in earlier rounds may since have become
            # direction-conflicting with a path accepted afterwards, so the
            # conflict check is re-applied here, right before selection,
            # instead of only once at insertion time.
            candidates = [
                (cost, candidate)
                for cost, candidate in candidates
                if not any(
                    (b, a) in directed_edges_used
                    for a, b in zip(candidate, candidate[1:])
                )
            ]
            if not candidates:
                break

            candidates.sort(key=lambda item: item[0])
            _, best_candidate = candidates.pop(0)
            accepted.append(best_candidate)
            directed_edges_used.update(zip(best_candidate, best_candidate[1:]))

        return accepted
