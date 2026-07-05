*This project has been created as part of the 42 curriculum by bgomes-b.*

# Fly-In

## Description

Fly-In is a multi-drone routing simulator. Given a map file describing a
network of zones (nodes) and connections (edges), it routes a whole fleet of
drones from a single start hub to a single end hub, turn by turn, while
respecting:

- **Zone occupancy limits** (`max_drones`) and **connection capacity limits**
  (`max_link_capacity`).
- **Zone-type movement costs**: `normal`/`priority` zones cost 1 turn to
  enter, `restricted` zones cost 2 turns (and cannot be paused mid-transit),
  `blocked` zones can never be entered.
- **Conflict-free scheduling**: two drones can never occupy the same zone (or
  traverse the same connection) beyond its capacity on the same turn.

The goal is to deliver every drone to the end hub in as few simulation turns
as possible, and to make that process visible through a colored terminal log
and/or a graphical replay window.

No graph library (`networkx`, `graphlib`, ...) is used anywhere: the graph
model, Dijkstra's algorithm and the alternative-path search are all
implemented from scratch in [fly_in/graph.py](fly_in/graph.py).

## Instructions

Requires **Python 3.10+**. No third-party package is strictly required to run
the simulation (the GUI uses the standard library's `tkinter`); `colorama` is
used only to make ANSI colors reliable on older Windows terminals.

```bash
make install   # pip install -r requirements.txt
make run       # runs main.py on a default map (MAP=<path> to override)
make debug     # runs main.py under pdb
make lint      # flake8 + mypy (project-standard flags)
make lint-strict  # flake8 + mypy --strict
make clean     # removes __pycache__, .mypy_cache, .pytest_cache
```

Running directly:

```bash
python main.py maps/hard/02_capacity_hell.txt          # colored terminal log
python main.py maps/medium/03_priority_puzzle.txt --gui # also opens a GUI replay
python main.py maps/easy/02_simple_fork.txt --gui --no-terminal
```

CLI options:

| Flag                 | Effect                                                        |
|-----------------------|----------------------------------------------------------------|
| `map_file` (positional)| Path to the map file (default: `maps/easy/01_linear_path.txt`) |
| `--gui`               | Opens a tkinter window to replay the simulation turn by turn   |
| `--no-terminal`       | Skips the colored terminal log                                 |
| `--path-pool-size`    | Force a specific number of alternative routes (default: auto-search, see below) |
| `--max-turns`         | Safety limit before aborting as a deadlock (default 5000)      |

Sample maps for every difficulty tier (as shipped with the subject) live
under [maps/](maps/). Tests (not graded, kept for our own confidence) can be
run with `pytest`.

## Algorithm & implementation strategy

The project is split into small, single-responsibility, fully-typed classes:

- **`fly_in.parser.MapParser`** — a hand-written, line-by-line parser that
  enforces every constraint from the subject (unique start/end hub, no
  dashes/spaces in zone names, valid zone types, positive capacities,
  duplicate connections, unknown metadata keys, etc.) and raises
  `MapParseError` with the offending line number and a clear reason.
- **`fly_in.graph.Graph`** — an adjacency-list graph. It implements:
  - **Dijkstra's algorithm** (`shortest_path`), using each zone's
    `path_weight` as the edge cost. `priority` zones are given a slightly
    lower weight (9) than `normal` zones (10) so that, at equal real turn
    cost (both take 1 turn), the search prefers them — `restricted` zones
    weigh 20, reflecting their real 2-turn cost. `blocked` zones are simply
    never expanded.
  - **`k_shortest_paths`** — a simplified version of Yen's algorithm: after
    accepting the cheapest path, it "spurs" an alternative route from every
    node of the most recently accepted path (banning only the edge already
    used from that node, not the whole path) to discover genuinely
    different routes. This is what lets drones be spread across separate
    branches (e.g. `path_a` vs `path_b` in a fork) even though they all
    share the same entry edge out of the start hub.
  - Candidate paths that would reuse an already-accepted edge in the
    **opposite direction** are rejected. Two drones crossing the same edge
    head-on, into single-capacity zones on both ends, can deadlock forever
    (neither can ever free the zone the other needs) — so the accepted pool
    is kept direction-consistent by construction instead of trying to detect
    and resolve that deadlock at simulation time.
- **`fly_in.simulation.Simulation`** — the turn-based scheduler:
  1. Drones are assigned one of the `path_pool_size` cheapest routes,
     round-robin, so the fleet is distributed instead of funneling through a
     single corridor.
  2. Every turn, in-flight drones (mid-transit toward a `restricted` zone)
     are advanced first; when their remaining transit time hits zero they
     arrive and their reservation becomes an actual occupancy.
  3. Remaining drones (closest to the goal first, to clear zones for others
     sooner) each try to advance to the next zone on their path: the move is
     committed only if the connection and the destination zone still have
     free capacity (accounting for drones that just vacated a zone this same
     turn, and for zones/connections already reserved by ongoing restricted
     transits). Otherwise the drone waits — this is the "strategic waiting"
     required by the subject, driven entirely by capacity, not guesswork.
  4. A drone entering a `restricted` zone leaves its origin immediately (so
     others can use it) but is only placed at the destination two turns
     later, and its destination/connection capacity is reserved for the
     whole transit so nobody can double-book it.
  5. A stall counter aborts with a clear `SimulationError` if too many
     consecutive turns produce no movement at all (a genuine deadlock in the
     map/assignment), rather than looping forever.
- **`fly_in.simulation.find_best_simulation`** — how many alternative routes
  (`path_pool_size`) drones are spread across changes *which* zones become
  congestion points, and that can shift the total turn count noticeably on
  capacity-heavy maps (sometimes for the better, sometimes for the worse —
  spreading drones too thin can be as costly as not spreading them at all).
  Since a full simulation run is cheap (well under 50ms even on the largest
  provided map), `main.py` runs the simulation once per pool size in
  `range(1, 13)` by default and keeps whichever run finished in the fewest
  turns, instead of requiring a hand-tuned constant. Pass `--path-pool-size`
  to skip the search and force a specific value.

**Complexity.** Dijkstra runs in `O(E log V)`; `k_shortest_paths` runs it up
to `O(k * V)` times, which is negligible for the map sizes involved (tens of
zones). Paths are computed **once** per run and then simply indexed into
turn by turn — no re-planning happens during the simulation itself. Each
simulation turn is `O(active drones)` (zone/connection lookups are `O(1)`
dict operations), so the whole run is `O(turns * drones)`. Memory usage is
`O(V + E + drones)` for the graph and fleet, plus one small snapshot per turn
for the replay/GUI (`O(turns * drones)` in the worst case, still tiny for the
provided maps).

**Benchmarks** (measured against the subject's reference targets):

| Map                          | Turns | Target |
|-------------------------------|-------|--------|
| Easy — linear path             | 4     | ≤ 6    |
| Easy — simple fork              | 4     | ≤ 8    |
| Easy — basic capacity           | 4     | ≤ 6    |
| Medium — dead end trap          | 8     | ≤ 12   |
| Medium — circular loop          | 15    | ≤ 15   |
| Medium — priority puzzle        | 7     | ≤ 12   |
| Hard — maze nightmare           | 13    | ≤ 30   |
| Hard — capacity hell            | 16    | ≤ 35   |
| Hard — ultimate challenge       | 26    | ≤ 45   |
| Challenger — The Impossible Dream (optional, bonus) | **43** | record 45 |

(All figures above are from `main.py`'s default auto-search over pool
sizes, i.e. `find_best_simulation`; the challenger map beats the reference
record of 45 turns.)

## Visual representation

Two visualization modes are provided (either can be used alone, or both
together):

- **Colored terminal output** (`fly_in/visualization/terminal_view.py`,
  default): each turn is printed as one line, with every drone move token
  colored according to the zone it is entering (the same free-form `color`
  metadata from the map file), using ANSI escape codes. This makes it easy
  to visually follow, at a glance, which drones are heading into
  `restricted` (often red) versus `priority` (often blue/cyan) zones without
  reading every token.
- **Graphical replay** (`fly_in/visualization/gui_view.py`, `--gui`): a
  `tkinter` window (ttk-themed, with a header, a legend and a status bar)
  draws every zone as a circle at its `(x, y)` map coordinates, filled with
  its declared color, connected by lines representing the connections.
  "Prev"/"Next"/"Play"/slider controls step through the turns the simulation
  already computed. On top of the basics, a few things were specifically
  built to make dense or slow-moving maps easier to read:
  - **Rank-based, screen-fitted layout**: zones are positioned by the *rank*
    of their `x`/`y` value (their index in the sorted list of distinct
    coordinates used anywhere in the map), not the raw coordinate — so a
    map that packs dozens of zones into a few numeric units on one axis
    while a couple of outliers sit far away doesn't get crushed. The gap
    between adjacent ranks is then stretched to fill as much of the actual
    screen as it can, but clamped so it never shrinks below a floor that
    keeps circles and labels readable (only the very densest map,
    `challenger`, still needs a scrollbar) nor grows enough to spread a
    small map out absurdly thin. Zone name labels alternate between two
    vertical offsets by column so close neighbors don't overlap.
  - **Zone-type rings**: independent of the map's own `color` metadata, a
    colored ring is drawn around each zone based on its `zone_type`
    (`priority` = teal, `restricted` = red, `blocked` = dashed gray with a
    strike-through), matching a legend panel, so the type is readable at a
    glance without memorizing colors.
  - **Distinct in-flight marker**: a drone that hasn't reached a zone yet
    (mid-transit toward a `restricted` zone, per VII.3: "the drone occupies
    the connection during transit") is drawn as a diamond, not a circle,
    on a dashed/arrowed highlight of the connection it's using — so the
    midpoint of an edge never reads as if a new permanent vertex appeared
    there. A drone resting in a zone is instead a small quadcopter glyph
    (body + four rotor arms).
  - **Animated transitions**: instead of jumping straight to its new spot,
    each drone marker eases from its previous screen position to the new
    one over a handful of frames, so stepping or auto-playing through
    turns reads as continuous motion along the network rather than a
    slideshow.

  This gives a spatial, at-a-glance understanding of congestion (e.g.
  drones piling up around a low-capacity gate) that a text log alone does
  not convey.

## Resources

- [Dijkstra's algorithm — Wikipedia](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
- [Yen's algorithm for k shortest loopless paths — Wikipedia](https://en.wikipedia.org/wiki/Yen%27s_algorithm)
- [Multi-Agent Path Finding — overview](https://en.wikipedia.org/wiki/Multi-agent_pathfinding)
- [Python `typing` module documentation](https://docs.python.org/3/library/typing.html)
- [mypy documentation](https://mypy.readthedocs.io/)
- [flake8 documentation](https://flake8.pycqa.org/)
- [tkinter documentation](https://docs.python.org/3/library/tkinter.html)

**How AI was used:** AI assistance (Claude) was used to help design the
turn-based scheduling algorithm and the direction-conflict fix for the
capacity-hell deadlock (a head-on swap between two drones through
single-capacity gates), to draft the parser's regular expressions, and to
generate boilerplate for the `tkinter` GUI replay. Every generated piece was
read, tested against all provided maps (including writing extra unit tests
in `tests/`), and manually adjusted — in particular the deadlock in
`hard/02_capacity_hell.txt` was diagnosed by hand (tracing the exact two
drones and zones involved) before deciding on the direction-consistency fix
in `Graph.k_shortest_paths`, rather than accepting a first suggestion at face
value. The `find_best_simulation` auto-search was added after empirically
observing (by hand, varying `path_pool_size` and logging the resulting turn
counts) that this single parameter shifted the challenger map anywhere from
43 to 54 turns non-monotonically — auto-searching a small range was a
simpler and more robust fix than trying to hand-derive the ideal value.
The GUI's rank-based/screen-fitted layout and the drone animation were also
AI-assisted and went through a few iterations based on visually running the
tool: the first version scaled zone positions directly off the map's
raw min/max coordinates, which looked fine on small maps but crushed dense
maps into overlapping circles; the fix (index-based rank instead of the raw
value, clamped to a screen-derived spacing range) was verified by measuring
the resulting content size against the actual screen size for every
provided map, not just eyeballing it once.
