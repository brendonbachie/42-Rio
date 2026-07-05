"""Domain models: zones, connections and drones."""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Optional


class ZoneType(str, Enum):
    """The kind of a zone, which determines its cost and accessibility."""

    NORMAL = "normal"
    BLOCKED = "blocked"
    RESTRICTED = "restricted"
    PRIORITY = "priority"


#: Number of simulation turns needed to enter a zone of a given type.
ZONE_TURN_COST = {
    ZoneType.NORMAL: 1,
    ZoneType.PRIORITY: 1,
    ZoneType.RESTRICTED: 2,
}

#: Cost used by the pathfinder. Priority zones are given a slightly lower
#: weight than normal zones so that, everything else being equal, the
#: shortest-path search prefers them, without changing their real 1-turn
#: movement cost.
ZONE_PATH_WEIGHT = {
    ZoneType.NORMAL: 10,
    ZoneType.PRIORITY: 9,
    ZoneType.RESTRICTED: 20,
}

#: Zones with unlimited capacity (start/end hubs) are modeled with this value.
UNLIMITED_CAPACITY = 10 ** 9


@dataclass
class Zone:
    """A single zone (node) of the drone network."""

    name: str
    x: int
    y: int
    zone_type: ZoneType = ZoneType.NORMAL
    color: Optional[str] = None
    max_drones: int = 1
    is_start: bool = False
    is_end: bool = False

    @property
    def capacity(self) -> int:
        """Effective simultaneous-drone capacity of this zone."""
        if self.is_start or self.is_end:
            return UNLIMITED_CAPACITY
        return self.max_drones

    @property
    def turn_cost(self) -> int:
        """Number of turns required to move into this zone."""
        return ZONE_TURN_COST.get(self.zone_type, 1)

    @property
    def path_weight(self) -> int:
        """Weight used by the pathfinder when entering this zone."""
        return ZONE_PATH_WEIGHT.get(self.zone_type, 10)


@dataclass
class Connection:
    """A bidirectional edge between two zones."""

    zone_a: str
    zone_b: str
    max_link_capacity: int = 1

    @property
    def key(self) -> str:
        """A direction-independent identifier for this connection."""
        return "-".join(sorted((self.zone_a, self.zone_b)))

    def other(self, zone_name: str) -> str:
        """Return the zone at the opposite end of this connection."""
        if zone_name == self.zone_a:
            return self.zone_b
        if zone_name == self.zone_b:
            return self.zone_a
        raise ValueError(
            f"Zone {zone_name!r} is not part of connection {self.key!r}"
        )


@dataclass
class TransitState:
    """Tracks a drone currently in flight toward a restricted zone."""

    connection_name: str
    destination: str
    turns_remaining: int


class Drone:
    """A single drone moving from the start hub to the end hub."""

    def __init__(self, drone_id: int, start_zone: str) -> None:
        self.id = drone_id
        self.path: list[str] = [start_zone]
        self.path_index: int = 0
        self.transit: Optional[TransitState] = None
        self.delivered: bool = False

    @property
    def position(self) -> str:
        """The zone the drone currently occupies (only valid when not in transit)."""
        return self.path[self.path_index]

    @property
    def next_zone(self) -> Optional[str]:
        """The next zone on this drone's assigned path, if any."""
        if self.path_index + 1 < len(self.path):
            return self.path[self.path_index + 1]
        return None

    @property
    def label(self) -> str:
        """The identifier used in the simulation output (e.g. ``D3``)."""
        return f"D{self.id}"

    def __repr__(self) -> str:
        return f"Drone({self.label}, at={self.position!r}, delivered={self.delivered})"
