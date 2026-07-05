"""Turn-based simulation engine that routes drones from start to end."""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, FrozenSet, Iterable, List, Optional, Set, Tuple

from .exceptions import SimulationError
from .graph import Graph
from .models import Drone, TransitState, ZoneType

ConnectionKey = FrozenSet[str]


@dataclass
class TurnSnapshot:
    """A frozen view of the world state at the end of a given turn, used by
    the visualization layer to replay the simulation."""

    turn: int
    moves: List[str]
    zone_residents: Dict[str, List[str]]
    in_transit: List[Tuple[str, str, str]]


@dataclass
class SimulationResult:
    """The full outcome of a simulation run."""

    log_lines: List[str]
    snapshots: List[TurnSnapshot]
    turns_per_drone: Dict[int, int]

    @property
    def total_turns(self) -> int:
        return len(self.log_lines)

    @property
    def average_turns_per_drone(self) -> float:
        if not self.turns_per_drone:
            return 0.0
        return sum(self.turns_per_drone.values()) / len(self.turns_per_drone)


@dataclass
class _DroneRuntime:
    """Mutable simulation-only state kept alongside a :class:`Drone`."""

    turns_taken: int = 0
    departure_turn: int = 0


class Simulation:
    """Simulates the fleet's journey from the start hub to the end hub.

    The scheduling strategy is intentionally simple: each drone is assigned
    one of the ``path_pool_size`` cheapest loop-less paths (round-robin, to
    spread drones across distinct routes), then every turn each drone
    greedily tries to advance to the next zone on its path, waiting whenever
    a zone or connection is at capacity. Zone occupancy and connection usage
    are tracked explicitly so that all capacity and restricted-zone transit
    rules from the subject are enforced.
    """

    def __init__(
        self,
        graph: Graph,
        nb_drones: int,
        path_pool_size: int = 6,
        max_turns: int = 5000,
    ) -> None:
        if graph.start_zone is None or graph.end_zone is None:
            raise SimulationError("graph is missing a start or end hub")
        self.graph = graph
        self.max_turns = max_turns
        self.path_pool_size = path_pool_size
        self.drones: List[Drone] = [
            Drone(drone_id, graph.start_zone) for drone_id in range(1, nb_drones + 1)
        ]
        self._runtime: Dict[int, _DroneRuntime] = {
            drone.id: _DroneRuntime() for drone in self.drones
        }
        self._zone_residents: Dict[str, Set[int]] = {
            graph.start_zone: {drone.id for drone in self.drones}
        }
        self._zone_reserved: Dict[str, int] = {}
        self._connection_reserved: Dict[ConnectionKey, int] = {}

    def run(self) -> SimulationResult:
        """Execute the simulation until every drone reaches the end hub."""
        self._assign_paths()

        log_lines: List[str] = []
        snapshots: List[TurnSnapshot] = []
        turn = 0
        idle_turns = 0
        stall_limit = max(10, len(self.drones) * 3)

        while not self._all_delivered():
            turn += 1
            if turn > self.max_turns:
                raise SimulationError(
                    f"simulation exceeded {self.max_turns} turns without "
                    "delivering every drone"
                )

            moves = self._process_turn(turn)
            log_lines.append(" ".join(moves))
            snapshots.append(self._snapshot(turn, moves))

            if moves:
                idle_turns = 0
            else:
                idle_turns += 1
                if idle_turns > stall_limit:
                    raise SimulationError(
                        "deadlock detected: no drone could move for "
                        f"{idle_turns} consecutive turns"
                    )

        turns_per_drone = {
            drone.id: self._runtime[drone.id].turns_taken for drone in self.drones
        }
        return SimulationResult(
            log_lines=log_lines, snapshots=snapshots, turns_per_drone=turns_per_drone
        )

    def _assign_paths(self) -> None:
        start, end = self.graph.start_zone, self.graph.end_zone
        assert start is not None and end is not None
        paths = self.graph.k_shortest_paths(start, end, self.path_pool_size)
        if not paths:
            raise SimulationError(
                f"no path exists between '{start}' and '{end}'"
            )
        for index, drone in enumerate(self.drones):
            chosen_path = paths[index % len(paths)]
            drone.path = list(chosen_path)
            drone.path_index = 0

    def _all_delivered(self) -> bool:
        return all(drone.delivered for drone in self.drones)

    def _process_turn(self, turn: int) -> List[str]:
        moves: List[str] = []

        transit_moves, handled_ids = self._advance_transits()
        moves.extend(transit_moves)

        active_drones = [
            drone
            for drone in self.drones
            if not drone.delivered
            and drone.transit is None
            and drone.id not in handled_ids
            and drone.next_zone is not None
        ]
        active_drones.sort(key=lambda d: (len(d.path) - d.path_index, d.id))

        connection_usage = dict(self._connection_reserved)

        for drone in active_drones:
            current = drone.position
            next_zone_name = drone.next_zone
            assert next_zone_name is not None
            connection = self.graph.get_connection(current, next_zone_name)
            conn_key: ConnectionKey = frozenset((connection.zone_a, connection.zone_b))
            used = connection_usage.get(conn_key, 0)
            if used >= connection.max_link_capacity:
                continue

            next_zone = self.graph.zones[next_zone_name]
            occupied = len(self._zone_residents.get(next_zone_name, set()))
            occupied += self._zone_reserved.get(next_zone_name, 0)
            if occupied >= next_zone.capacity:
                continue

            self._zone_residents.setdefault(current, set()).discard(drone.id)
            connection_usage[conn_key] = used + 1

            if next_zone.zone_type == ZoneType.RESTRICTED:
                self._connection_reserved[conn_key] = (
                    self._connection_reserved.get(conn_key, 0) + 1
                )
                self._zone_reserved[next_zone_name] = (
                    self._zone_reserved.get(next_zone_name, 0) + 1
                )
                drone.transit = TransitState(
                    connection_name=f"{current}-{next_zone_name}",
                    destination=next_zone_name,
                    turns_remaining=next_zone.turn_cost - 1,
                )
                moves.append(f"{drone.label}-{drone.transit.connection_name}")
            else:
                self._zone_residents.setdefault(next_zone_name, set()).add(drone.id)
                drone.path_index += 1
                self._runtime[drone.id].turns_taken += 1
                moves.append(f"{drone.label}-{next_zone_name}")
                if next_zone_name == self.graph.end_zone:
                    drone.delivered = True

        return moves

    def _advance_transits(self) -> Tuple[List[str], Set[int]]:
        moves: List[str] = []
        handled_ids: Set[int] = set()
        for drone in self.drones:
            transit = drone.transit
            if transit is None:
                continue

            handled_ids.add(drone.id)
            self._runtime[drone.id].turns_taken += 1
            transit.turns_remaining -= 1
            if transit.turns_remaining > 0:
                moves.append(f"{drone.label}-{transit.connection_name}")
                continue

            destination = transit.destination
            drone.path_index += 1
            self._zone_reserved[destination] = max(
                0, self._zone_reserved.get(destination, 0) - 1
            )
            self._zone_residents.setdefault(destination, set()).add(drone.id)
            drone.transit = None
            moves.append(f"{drone.label}-{destination}")

            if destination == self.graph.end_zone:
                drone.delivered = True
        self._release_completed_reservations()
        return moves, handled_ids

    def _release_completed_reservations(self) -> None:
        # Connection reservations are released as soon as the owning drone's
        # transit ends; recomputed from scratch to stay simple and correct.
        active: Dict[ConnectionKey, int] = {}
        for drone in self.drones:
            if drone.transit is None:
                continue
            zone_a, _, zone_b = drone.transit.connection_name.partition("-")
            # connection_name is "origin-destination" and neither zone
            # contains dashes, so a simple partition on the first dash works
            # only when the origin has no dash -- true by parser constraints.
            key = frozenset((zone_a, drone.transit.destination))
            active[key] = active.get(key, 0) + 1
        self._connection_reserved = active

    def _snapshot(self, turn: int, moves: List[str]) -> TurnSnapshot:
        zone_residents = {
            zone: sorted(f"D{drone_id}" for drone_id in drone_ids)
            for zone, drone_ids in self._zone_residents.items()
            if drone_ids
        }
        in_transit = [
            (drone.label, drone.transit.connection_name, drone.transit.destination)
            for drone in self.drones
            if drone.transit is not None
        ]
        return TurnSnapshot(
            turn=turn, moves=list(moves), zone_residents=zone_residents, in_transit=in_transit
        )


#: Pool sizes tried by :func:`find_best_simulation`. How many alternative
#: routes drones get spread across changes which zones/connections become
#: congestion points, which in turn can noticeably shift the total turn
#: count in either direction on capacity-heavy maps. Trying a handful of
#: small values and keeping the fastest run is cheap (each simulation runs
#: in well under a second, even on the largest provided map) and removes the
#: need to hand-tune this parameter per map.
DEFAULT_POOL_SIZES: Tuple[int, ...] = tuple(range(1, 13))


def find_best_simulation(
    graph: Graph,
    nb_drones: int,
    pool_sizes: Iterable[int] = DEFAULT_POOL_SIZES,
    max_turns: int = 5000,
) -> SimulationResult:
    """Run the simulation once per candidate pool size and keep the best.

    "Best" means the fewest total turns among the runs that successfully
    deliver every drone; pool sizes that lead to a deadlock are skipped.

    Raises:
        SimulationError: If every candidate pool size fails.
    """
    best_result: Optional[SimulationResult] = None
    last_error: Optional[SimulationError] = None

    for pool_size in pool_sizes:
        try:
            result = Simulation(
                graph, nb_drones, path_pool_size=pool_size, max_turns=max_turns
            ).run()
        except SimulationError as error:
            last_error = error
            continue
        if best_result is None or result.total_turns < best_result.total_turns:
            best_result = result

    if best_result is None:
        raise last_error or SimulationError("no candidate pool size produced a valid run")
    return best_result
