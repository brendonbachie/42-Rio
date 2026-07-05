"""Tests for fly_in.parser.MapParser."""

from pathlib import Path

import pytest

from fly_in.exceptions import MapParseError
from fly_in.models import ZoneType
from fly_in.parser import MapParser


def write_map(tmp_path: Path, content: str) -> str:
    map_file = tmp_path / "map.txt"
    map_file.write_text(content, encoding="utf-8")
    return str(map_file)


VALID_MAP = """
# a small valid map
nb_drones: 3
start_hub: base 0 0 [color=green]
hub: relay 1 0 [zone=priority color=blue max_drones=2]
hub: tower 2 0 [zone=restricted color=red]
end_hub: dest 3 0 [color=yellow]
connection: base-relay [max_link_capacity=2]
connection: relay-tower
connection: tower-dest
"""


def test_parses_valid_map(tmp_path: Path) -> None:
    nb_drones, graph = MapParser().parse(write_map(tmp_path, VALID_MAP))

    assert nb_drones == 3
    assert graph.start_zone == "base"
    assert graph.end_zone == "dest"
    assert graph.zones["relay"].zone_type == ZoneType.PRIORITY
    assert graph.zones["relay"].max_drones == 2
    assert graph.zones["tower"].zone_type == ZoneType.RESTRICTED
    connection = graph.get_connection("base", "relay")
    assert connection.max_link_capacity == 2


def test_max_drones_ignored_on_start_and_end(tmp_path: Path) -> None:
    content = """
    nb_drones: 1
    start_hub: base 0 0 [max_drones=5]
    end_hub: dest 1 0 [max_drones=5]
    connection: base-dest
    """
    _, graph = MapParser().parse(write_map(tmp_path, content))
    assert graph.zones["base"].capacity > 5
    assert graph.zones["dest"].capacity > 5


@pytest.mark.parametrize(
    "content,expected_message_fragment",
    [
        ("start_hub: base 0 0\nend_hub: dest 1 0\nconnection: base-dest\n", "nb_drones"),
        (
            "nb_drones: 1\nstart_hub: base 0 0\nstart_hub: other 1 0\n"
            "end_hub: dest 2 0\nconnection: base-dest\n",
            "duplicate start_hub",
        ),
        (
            "nb_drones: 1\nstart_hub: base 0 0\nend_hub: dest 1 0\n"
            "hub: weird 2 0 [zone=unknown]\nconnection: base-dest\n",
            "invalid zone type",
        ),
        (
            "nb_drones: 1\nstart_hub: base 0 0\nend_hub: dest 1 0\n"
            "connection: base-ghost\n",
            "undefined zone",
        ),
        (
            "nb_drones: 1\nstart_hub: base 0 0\nend_hub: dest 1 0\n"
            "connection: base-dest\nconnection: dest-base\n",
            "duplicate connection",
        ),
        (
            "nb_drones: 1\nstart_hub: base 0 0\nend_hub: dest 1 0\n"
            "hub: a-b 2 0\nconnection: base-dest\n",
            "dashes",
        ),
        (
            "nb_drones: 1\nstart_hub: base 0 0\nend_hub: dest 1 0\n"
            "connection: base-dest [max_link_capacity=0]\n",
            "positive integer",
        ),
        (
            "nb_drones: 0\nstart_hub: base 0 0\nend_hub: dest 1 0\n"
            "connection: base-dest\n",
            "positive integer",
        ),
    ],
)
def test_reports_parse_errors(
    tmp_path: Path, content: str, expected_message_fragment: str
) -> None:
    with pytest.raises(MapParseError) as excinfo:
        MapParser().parse(write_map(tmp_path, content))
    assert expected_message_fragment in str(excinfo.value)
