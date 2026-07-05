"""Parser for the Fly-In map file format (see subject chapter VI/VII.4)."""

from __future__ import annotations

import re
from typing import Dict, FrozenSet, Tuple

from .exceptions import MapParseError
from .graph import Graph
from .models import Connection, Zone, ZoneType

_ZONE_LINE = re.compile(
    r"^(?P<kind>start_hub|end_hub|hub):\s*"
    r"(?P<name>\S+)\s+(?P<x>\S+)\s+(?P<y>\S+)"
    r"(?:\s*\[(?P<meta>[^\]]*)\])?\s*$"
)
_CONNECTION_LINE = re.compile(
    r"^connection:\s*(?P<pair>[^\s\[\]]+)(?:\s*\[(?P<meta>[^\]]*)\])?\s*$"
)
_NB_DRONES_LINE = re.compile(r"^nb_drones:\s*(?P<value>\S+)\s*$")
_TOKEN = re.compile(r"^[A-Za-z0-9_]+=[^\s=]+$")
_VALID_ZONE_TYPES = {zone_type.value for zone_type in ZoneType}


class MapParser:
    """Parses a Fly-In map file into a drone count and a :class:`Graph`."""

    def parse(self, file_path: str) -> Tuple[int, Graph]:
        """Read and validate ``file_path``, returning ``(nb_drones, graph)``.

        Raises:
            MapParseError: If the file violates any parser constraint.
        """
        with open(file_path, "r", encoding="utf-8") as handle:
            lines = handle.readlines()

        graph = Graph()
        nb_drones: int = 0
        nb_drones_seen = False
        start_seen = False
        end_seen = False
        seen_connections: Dict[FrozenSet[str], int] = {}

        for line_number, raw_line in enumerate(lines, start=1):
            line = raw_line.split("#", 1)[0].strip()
            if not line:
                continue

            if not nb_drones_seen:
                match = _NB_DRONES_LINE.match(line)
                if not match:
                    raise MapParseError(
                        line_number, "expected 'nb_drones: <positive_integer>' first"
                    )
                nb_drones = self._parse_positive_int(
                    match.group("value"), line_number, "nb_drones"
                )
                nb_drones_seen = True
                continue

            zone_match = _ZONE_LINE.match(line)
            if zone_match:
                kind = zone_match.group("kind")
                if kind == "start_hub":
                    if start_seen:
                        raise MapParseError(line_number, "duplicate start_hub")
                    start_seen = True
                elif kind == "end_hub":
                    if end_seen:
                        raise MapParseError(line_number, "duplicate end_hub")
                    end_seen = True
                zone = self._parse_zone(zone_match, kind, line_number)
                if zone.name in graph.zones:
                    raise MapParseError(
                        line_number, f"duplicate zone name '{zone.name}'"
                    )
                graph.add_zone(zone)
                continue

            connection_match = _CONNECTION_LINE.match(line)
            if connection_match:
                connection = self._parse_connection(
                    connection_match, graph, line_number
                )
                edge_key = frozenset((connection.zone_a, connection.zone_b))
                if edge_key in seen_connections:
                    raise MapParseError(
                        line_number,
                        f"duplicate connection between "
                        f"'{connection.zone_a}' and '{connection.zone_b}'",
                    )
                seen_connections[edge_key] = line_number
                graph.add_connection(connection)
                continue

            raise MapParseError(line_number, f"unrecognized line: {raw_line.strip()!r}")

        if not nb_drones_seen:
            raise MapParseError(1, "missing 'nb_drones: <positive_integer>' line")
        if not start_seen:
            raise MapParseError(len(lines), "missing a start_hub zone")
        if not end_seen:
            raise MapParseError(len(lines), "missing an end_hub zone")

        return nb_drones, graph

    def _parse_zone(self, match: "re.Match[str]", kind: str, line_number: int) -> Zone:
        name = match.group("name")
        self._validate_zone_name(name, line_number)
        x = self._parse_int(match.group("x"), line_number, "x coordinate")
        y = self._parse_int(match.group("y"), line_number, "y coordinate")
        metadata = self._parse_metadata(match.group("meta"), line_number)

        is_start = kind == "start_hub"
        is_end = kind == "end_hub"

        zone_type = ZoneType.NORMAL
        if "zone" in metadata:
            raw_type = metadata.pop("zone")
            if raw_type not in _VALID_ZONE_TYPES:
                raise MapParseError(
                    line_number,
                    f"invalid zone type '{raw_type}', expected one of "
                    f"{sorted(_VALID_ZONE_TYPES)}",
                )
            zone_type = ZoneType(raw_type)

        color = metadata.pop("color", None)

        max_drones = 1
        if "max_drones" in metadata:
            raw_max = metadata.pop("max_drones")
            parsed_max = self._parse_positive_int(raw_max, line_number, "max_drones")
            if not (is_start or is_end):
                max_drones = parsed_max
            # max_drones on start_hub/end_hub is parsed but ignored, per spec.

        if metadata:
            unknown = ", ".join(sorted(metadata))
            raise MapParseError(line_number, f"unknown zone metadata key(s): {unknown}")

        return Zone(
            name=name,
            x=x,
            y=y,
            zone_type=zone_type,
            color=color,
            max_drones=max_drones,
            is_start=is_start,
            is_end=is_end,
        )

    def _parse_connection(
        self, match: "re.Match[str]", graph: Graph, line_number: int
    ) -> Connection:
        pair = match.group("pair")
        if "-" not in pair:
            raise MapParseError(
                line_number, f"malformed connection '{pair}', expected '<zone1>-<zone2>'"
            )
        zone_a, _, zone_b = pair.partition("-")
        if not zone_a or not zone_b:
            raise MapParseError(
                line_number, f"malformed connection '{pair}', expected '<zone1>-<zone2>'"
            )
        for zone_name in (zone_a, zone_b):
            if not graph.has_zone(zone_name):
                raise MapParseError(
                    line_number,
                    f"connection references undefined zone '{zone_name}'",
                )
        if zone_a == zone_b:
            raise MapParseError(line_number, f"self-connection on zone '{zone_a}'")

        metadata = self._parse_metadata(match.group("meta"), line_number)
        max_link_capacity = 1
        if "max_link_capacity" in metadata:
            raw_capacity = metadata.pop("max_link_capacity")
            max_link_capacity = self._parse_positive_int(
                raw_capacity, line_number, "max_link_capacity"
            )

        if metadata:
            unknown = ", ".join(sorted(metadata))
            raise MapParseError(
                line_number, f"unknown connection metadata key(s): {unknown}"
            )

        return Connection(zone_a=zone_a, zone_b=zone_b, max_link_capacity=max_link_capacity)

    def _parse_metadata(self, raw: "str | None", line_number: int) -> Dict[str, str]:
        if not raw:
            return {}
        metadata: Dict[str, str] = {}
        for token in raw.split():
            if not _TOKEN.match(token):
                raise MapParseError(line_number, f"malformed metadata token '{token}'")
            key, _, value = token.partition("=")
            if key in metadata:
                raise MapParseError(line_number, f"duplicate metadata key '{key}'")
            metadata[key] = value
        return metadata

    def _validate_zone_name(self, name: str, line_number: int) -> None:
        if "-" in name:
            raise MapParseError(
                line_number, f"zone name '{name}' must not contain dashes"
            )

    def _parse_int(self, raw: str, line_number: int, field: str) -> int:
        try:
            return int(raw)
        except ValueError as exc:
            raise MapParseError(
                line_number, f"expected an integer for {field}, got '{raw}'"
            ) from exc

    def _parse_positive_int(self, raw: str, line_number: int, field: str) -> int:
        value = self._parse_int(raw, line_number, field)
        if value <= 0:
            raise MapParseError(
                line_number, f"expected a positive integer for {field}, got {value}"
            )
        return value
