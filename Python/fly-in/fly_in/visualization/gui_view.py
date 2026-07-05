"""Tkinter graphical replay of a completed simulation.

Still a single built-in library (no extra install), but styled with ttk
theming, a header/legend/status layout, a scrubber slider and small visual
touches (shadows, per-zone-type rings, a rotor-icon drone marker) instead of
plain default-tk widgets and bare circles.

Two things this module specifically works around:

- Zones are laid out by the *rank* of their x/y coordinates (their position
  in the sorted list of distinct values used anywhere in the map), not by
  the raw coordinate values. Some maps pack dozens of zones into a handful
  of numeric units on one axis while spreading a few outliers far away on
  the other; scaling directly off (min, max) would crush those dense zones
  into overlapping circles and labels. Rank-based spacing gives every
  adjacent pair of zones the same pixel gap regardless of the underlying
  numbers. That gap itself is chosen per map: as large as fits the actual
  screen (so small maps don't need scrolling), but never below a minimum
  that keeps circles and labels from overlapping (so large maps shrink to
  fit first, and only fall back to scrollbars once shrinking further would
  make them unreadable).
- Drone markers ease-interpolate from their previous screen position to
  their new one over a handful of animation frames instead of jumping
  there instantly, so stepping or playing through turns reads as motion
  along the network rather than a slideshow.
"""

from __future__ import annotations

import tkinter as tk
from tkinter import ttk
from typing import Dict, List, Optional, Tuple

from ..graph import Graph
from ..models import ZoneType
from ..simulation import SimulationResult

_ZONE_RADIUS = 22
_CANVAS_PADDING = 70
_MIN_SPACING = 70
_MAX_SPACING = 130
_SCREEN_FRACTION = 0.9
_CHROME_WIDTH = 260
_CHROME_HEIGHT = 190
_DEFAULT_ZONE_COLOR = "#d9d9d9"
_DRONE_RADIUS = 7
_PLAY_DELAY_MS = 700
_ANIMATION_STEPS = 12
_ANIMATION_DELAY_MS = 18
_GRID_STEP = 40

_BG_APP = "#eef1f6"
_BG_HEADER = "#1d3557"
_FG_HEADER = "#ffffff"
_ACCENT = "#ffb703"
_DRONE_COLOR = "#023047"
_GRID_COLOR = "#e3e7ee"
_LINE_COLOR = "#98a4b3"
_SHADOW_COLOR = "#c9ccd1"

_TYPE_RING_COLOR = {
    ZoneType.PRIORITY: "#2a9d8f",
    ZoneType.RESTRICTED: "#e63946",
    ZoneType.BLOCKED: "#6c757d",
}
_TYPE_LABEL = {
    ZoneType.NORMAL: "Normal zone",
    ZoneType.PRIORITY: "Priority zone (1 turn, preferred)",
    ZoneType.RESTRICTED: "Restricted zone (2 turns)",
    ZoneType.BLOCKED: "Blocked zone (inaccessible)",
}

#: A drone's on-canvas position: (x, y, is_in_flight).
DronePosition = Tuple[float, float, bool]
#: One in-flight drone: (label, connection identifier, destination zone).
InTransitEntry = Tuple[str, str, str]


class GuiView:
    """Replays a completed simulation on a tkinter canvas, turn by turn.

    The graph is drawn once (zones as colored circles positioned by their
    ``x``/``y`` coordinates, connections as lines between them); drone
    markers then glide to their new position for whichever turn is
    selected, either by stepping manually, dragging the turn slider, or
    pressing "Play".
    """

    def __init__(self, graph: Graph, result: SimulationResult) -> None:
        self.graph = graph
        self.result = result
        self.current_turn_index = -1
        self._playing = False
        self._animating = False
        self._updating_scale = False
        self._drone_items: List[int] = []
        self._current_positions: Optional[Dict[str, DronePosition]] = None

        self.root = tk.Tk()
        self.root.title("Fly-In — Drone Routing Simulation")
        self.root.configure(bg=_BG_APP)
        self.root.minsize(760, 560)

        target_w, target_h = self._target_viewport_size()
        self._positions, self.content_width, self.content_height = (
            self._compute_positions(target_w, target_h)
        )
        viewport_w = min(self.content_width, target_w)
        viewport_h = min(self.content_height, target_h)

        self._init_style()
        self._build_header()

        body = tk.Frame(self.root, bg=_BG_APP)
        body.pack(fill=tk.BOTH, expand=True)

        self.canvas = self._build_canvas(body, viewport_w, viewport_h)

        legend = tk.Frame(body, bg=_BG_APP)
        legend.pack(side=tk.LEFT, fill=tk.Y, padx=(6, 12), pady=12)
        self._build_legend(legend)

        self.status_var = tk.StringVar()
        ttk.Label(
            self.root, textvariable=self.status_var, style="Status.TLabel", anchor="w"
        ).pack(fill=tk.X, padx=14)

        self._build_controls()

        self._draw_static_network()
        self._render_state()

    def run(self) -> None:
        """Start the tkinter event loop (blocks until the window is closed)."""
        self.root.mainloop()

    # -- one-time UI construction ------------------------------------------------

    def _init_style(self) -> None:
        style = ttk.Style(self.root)
        try:
            style.theme_use("clam")
        except tk.TclError:
            pass
        style.configure("Fly.TButton", padding=(10, 6), font=("Segoe UI", 10))
        style.map("Fly.TButton", background=[("active", _ACCENT)])
        style.configure(
            "Header.TLabel",
            background=_BG_HEADER,
            foreground=_FG_HEADER,
            font=("Segoe UI", 14, "bold"),
        )
        style.configure(
            "Stat.TLabel",
            background=_BG_HEADER,
            foreground=_FG_HEADER,
            font=("Segoe UI", 10),
        )
        style.configure("Status.TLabel", background=_BG_APP, font=("Segoe UI", 9))
        style.configure("Fly.Horizontal.TScale", background=_BG_APP)

    def _build_header(self) -> None:
        header = tk.Frame(self.root, bg=_BG_HEADER)
        header.pack(fill=tk.X)
        ttk.Label(
            header, text="Fly-In — Drone Routing Simulation", style="Header.TLabel"
        ).pack(side=tk.LEFT, padx=16, pady=12)
        self.stats_var = tk.StringVar()
        ttk.Label(header, textvariable=self.stats_var, style="Stat.TLabel").pack(
            side=tk.RIGHT, padx=16
        )

    def _build_canvas(self, parent: tk.Frame, viewport_w: int, viewport_h: int) -> tk.Canvas:
        canvas_frame = tk.Frame(parent, bg=_BG_APP)
        canvas_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(12, 6), pady=12)
        canvas_frame.rowconfigure(0, weight=1)
        canvas_frame.columnconfigure(0, weight=1)

        canvas = tk.Canvas(
            canvas_frame,
            width=viewport_w,
            height=viewport_h,
            bg="white",
            highlightthickness=1,
            highlightbackground="#c3cad6",
            scrollregion=(0, 0, self.content_width, self.content_height),
        )
        h_scroll = ttk.Scrollbar(canvas_frame, orient=tk.HORIZONTAL, command=canvas.xview)
        v_scroll = ttk.Scrollbar(canvas_frame, orient=tk.VERTICAL, command=canvas.yview)
        canvas.configure(xscrollcommand=h_scroll.set, yscrollcommand=v_scroll.set)

        canvas.grid(row=0, column=0, sticky="nsew")
        v_scroll.grid(row=0, column=1, sticky="ns")
        h_scroll.grid(row=1, column=0, sticky="ew")
        return canvas

    def _build_legend(self, parent: tk.Frame) -> None:
        tk.Label(
            parent, text="Zone types", bg=_BG_APP, font=("Segoe UI", 10, "bold")
        ).pack(anchor="w", pady=(0, 6))
        for zone_type, text in _TYPE_LABEL.items():
            self._legend_row(
                parent, text, ring_color=_TYPE_RING_COLOR.get(zone_type), shape="ring"
            )

        tk.Label(
            parent, text="Drones", bg=_BG_APP, font=("Segoe UI", 10, "bold")
        ).pack(anchor="w", pady=(16, 6))
        self._legend_row(parent, "Resting in a zone", shape="drone")
        self._legend_row(parent, "In flight (2-turn transit)", shape="diamond")

    def _legend_row(
        self,
        parent: tk.Frame,
        text: str,
        ring_color: Optional[str] = None,
        shape: str = "ring",
    ) -> None:
        row = tk.Frame(parent, bg=_BG_APP)
        row.pack(anchor="w", pady=3)
        swatch = tk.Canvas(
            row, width=24, height=24, bg=_BG_APP, highlightthickness=0
        )
        swatch.pack(side=tk.LEFT, padx=(0, 8))
        if shape == "ring":
            swatch.create_oval(4, 4, 20, 20, fill="#f1f1f1", outline="black")
            if ring_color is not None:
                if ring_color == _TYPE_RING_COLOR[ZoneType.BLOCKED]:
                    swatch.create_oval(2, 2, 22, 22, outline=ring_color, width=2, dash=(3, 2))
                else:
                    swatch.create_oval(2, 2, 22, 22, outline=ring_color, width=2)
        elif shape == "drone":
            swatch.create_oval(7, 7, 17, 17, fill=_DRONE_COLOR, outline="white")
        else:
            swatch.create_polygon(
                12, 3, 21, 12, 12, 21, 3, 12, fill=_ACCENT, outline="white"
            )
        tk.Label(
            row, text=text, bg=_BG_APP, font=("Segoe UI", 9), wraplength=170, justify=tk.LEFT
        ).pack(side=tk.LEFT)

    def _build_controls(self) -> None:
        controls = tk.Frame(self.root, bg=_BG_APP)
        controls.pack(fill=tk.X, padx=14, pady=(4, 14))

        ttk.Button(
            controls, text="⏮ Prev", style="Fly.TButton", command=self.previous_turn
        ).pack(side=tk.LEFT)
        self.play_button = ttk.Button(
            controls, text="▶ Play", style="Fly.TButton", command=self.toggle_play
        )
        self.play_button.pack(side=tk.LEFT, padx=6)
        ttk.Button(
            controls, text="Next ⏭", style="Fly.TButton", command=self.next_turn
        ).pack(side=tk.LEFT)

        self.turn_scale = ttk.Scale(
            controls,
            from_=-1,
            to=max(len(self.result.snapshots) - 1, 0),
            orient=tk.HORIZONTAL,
            style="Fly.Horizontal.TScale",
            command=self._on_scale,
        )
        self.turn_scale.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=14)

    # -- static network ------------------------------------------------------

    def _target_viewport_size(self) -> Tuple[int, int]:
        # How much room the network drawing can realistically use: a
        # fraction of the actual screen, minus the header/legend/status/
        # controls chrome around it, so the window fits without the user
        # having to resize or scroll it themselves on an ordinary map.
        screen_w = self.root.winfo_screenwidth()
        screen_h = self.root.winfo_screenheight()
        target_w = max(int(screen_w * _SCREEN_FRACTION) - _CHROME_WIDTH, 360)
        target_h = max(int(screen_h * _SCREEN_FRACTION) - _CHROME_HEIGHT, 300)
        return target_w, target_h

    def _compute_positions(
        self, target_w: int, target_h: int
    ) -> Tuple[Dict[str, Tuple[float, float]], int, int]:
        # Rank-based, not value-based: adjacent distinct coordinates always
        # get the same pixel gap, so dense clusters of zones never get
        # crushed together just because a couple of outliers use large
        # coordinate values elsewhere on the map. That gap is stretched to
        # fill the available screen space, but clamped so it never shrinks
        # enough to make circles/labels overlap, nor grows enough to spread
        # a small map out absurdly thin.
        xs_sorted = sorted({zone.x for zone in self.graph.zones.values()})
        ys_sorted = sorted({zone.y for zone in self.graph.zones.values()})
        x_rank = {x: i for i, x in enumerate(xs_sorted)}
        y_rank = {y: i for i, y in enumerate(ys_sorted)}
        max_y_rank = len(ys_sorted) - 1
        self._x_rank = x_rank

        available_w = max(target_w - 2 * _CANVAS_PADDING, 0)
        available_h = max(target_h - 2 * _CANVAS_PADDING, 0)
        raw_column_spacing = available_w / max(len(xs_sorted) - 1, 1)
        raw_row_spacing = available_h / max(len(ys_sorted) - 1, 1)
        column_spacing = min(max(raw_column_spacing, _MIN_SPACING), _MAX_SPACING)
        row_spacing = min(max(raw_row_spacing, _MIN_SPACING), _MAX_SPACING)

        positions: Dict[str, Tuple[float, float]] = {}
        for zone in self.graph.zones.values():
            px = _CANVAS_PADDING + x_rank[zone.x] * column_spacing
            py = _CANVAS_PADDING + (max_y_rank - y_rank[zone.y]) * row_spacing
            positions[zone.name] = (px, py)

        content_w = int(_CANVAS_PADDING * 2 + max(len(xs_sorted) - 1, 0) * column_spacing)
        content_h = int(_CANVAS_PADDING * 2 + max(len(ys_sorted) - 1, 0) * row_spacing)
        return positions, content_w, content_h

    def _draw_static_network(self) -> None:
        self._draw_grid()

        for connection in self.graph.connections.values():
            x1, y1 = self._positions[connection.zone_a]
            x2, y2 = self._positions[connection.zone_b]
            self.canvas.create_line(
                x1, y1, x2, y2, fill=_LINE_COLOR, width=2, capstyle=tk.ROUND
            )
            if connection.max_link_capacity > 1:
                mx, my = (x1 + x2) / 2, (y1 + y2) / 2
                self.canvas.create_text(
                    mx, my - 9, text=f"×{connection.max_link_capacity}",
                    font=("Segoe UI", 7), fill=_LINE_COLOR,
                )

        for zone in self.graph.zones.values():
            x, y = self._positions[zone.name]
            self.canvas.create_oval(
                x - _ZONE_RADIUS + 3, y - _ZONE_RADIUS + 4,
                x + _ZONE_RADIUS + 3, y + _ZONE_RADIUS + 4,
                fill=_SHADOW_COLOR, outline="",
            )
            self._create_oval_safely(
                x - _ZONE_RADIUS, y - _ZONE_RADIUS,
                x + _ZONE_RADIUS, y + _ZONE_RADIUS,
                zone.color,
            )

            ring_color = _TYPE_RING_COLOR.get(zone.zone_type)
            if ring_color is not None:
                if zone.zone_type == ZoneType.BLOCKED:
                    self.canvas.create_oval(
                        x - _ZONE_RADIUS - 4, y - _ZONE_RADIUS - 4,
                        x + _ZONE_RADIUS + 4, y + _ZONE_RADIUS + 4,
                        outline=ring_color, width=2, dash=(3, 2),
                    )
                    span = _ZONE_RADIUS * 0.7
                    self.canvas.create_line(
                        x - span, y - span, x + span, y + span, fill=ring_color, width=2
                    )
                    self.canvas.create_line(
                        x - span, y + span, x + span, y - span, fill=ring_color, width=2
                    )
                else:
                    self.canvas.create_oval(
                        x - _ZONE_RADIUS - 4, y - _ZONE_RADIUS - 4,
                        x + _ZONE_RADIUS + 4, y + _ZONE_RADIUS + 4,
                        outline=ring_color, width=2,
                    )

            if zone.is_start:
                suffix = " (start)"
            elif zone.is_end:
                suffix = " (end)"
            elif zone.max_drones > 1:
                suffix = f" (max {zone.max_drones})"
            else:
                suffix = ""
            # Alternate the label between two vertical offsets by column
            # parity: on dense maps, horizontally adjacent zones are close
            # enough that same-height labels would run into each other.
            label_offset = 14 if self._x_rank[zone.x] % 2 == 0 else 28
            self.canvas.create_text(
                x, y + _ZONE_RADIUS + label_offset,
                text=f"{zone.name}{suffix}", font=("Segoe UI", 8),
            )

    def _draw_grid(self) -> None:
        for x in range(0, self.content_width, _GRID_STEP):
            self.canvas.create_line(x, 0, x, self.content_height, fill=_GRID_COLOR)
        for y in range(0, self.content_height, _GRID_STEP):
            self.canvas.create_line(0, y, self.content_width, y, fill=_GRID_COLOR)

    def _create_oval_safely(
        self, x1: float, y1: float, x2: float, y2: float, color: "str | None"
    ) -> int:
        fill = color or _DEFAULT_ZONE_COLOR
        try:
            return self.canvas.create_oval(x1, y1, x2, y2, fill=fill, outline="black", width=2)
        except tk.TclError:
            # The map file allows arbitrary color names; fall back quietly
            # if tkinter does not recognize one (e.g. "rainbow").
            return self.canvas.create_oval(
                x1, y1, x2, y2, fill=_DEFAULT_ZONE_COLOR, outline="black", width=2
            )

    # -- turn navigation ------------------------------------------------------

    def next_turn(self) -> None:
        """Advance the replay by one turn, if not already at the end."""
        if self._animating:
            return
        if self.current_turn_index < len(self.result.snapshots) - 1:
            self.current_turn_index += 1
            self._render_state()

    def previous_turn(self) -> None:
        """Rewind the replay by one turn, if not already at the start."""
        if self._animating:
            return
        if self.current_turn_index > -1:
            self.current_turn_index -= 1
            self._render_state()

    def toggle_play(self) -> None:
        """Start or pause automatic playback of the remaining turns."""
        self._playing = not self._playing
        self.play_button.config(text="⏸ Pause" if self._playing else "▶ Play")
        if self._playing:
            self._play_step()

    def _play_step(self) -> None:
        if not self._playing:
            return
        if self.current_turn_index >= len(self.result.snapshots) - 1:
            self._playing = False
            self.play_button.config(text="▶ Play")
            return
        self.next_turn()
        self.root.after(_PLAY_DELAY_MS, self._play_step)

    def _on_scale(self, raw_value: str) -> None:
        if self._updating_scale or self._animating:
            return
        index = round(float(raw_value))
        if index != self.current_turn_index:
            self.current_turn_index = index
            self._render_state()

    # -- per-turn rendering ------------------------------------------------------

    def _render_state(self) -> None:
        if self.current_turn_index == -1:
            assert self.graph.start_zone is not None
            all_ids = sorted(self.result.turns_per_drone)
            zone_residents: Dict[str, List[str]] = {
                self.graph.start_zone: [f"D{drone_id}" for drone_id in all_ids]
            }
            in_transit: List[InTransitEntry] = []
            header = "Before turn 1 (all drones at the start hub)"
            moves_label = ""
        else:
            snapshot = self.result.snapshots[self.current_turn_index]
            zone_residents = snapshot.zone_residents
            in_transit = snapshot.in_transit
            header = f"Turn {snapshot.turn} / {len(self.result.snapshots)}"
            moves_label = " ".join(snapshot.moves) if snapshot.moves else "(no movement)"

        self.status_var.set(f"{header}    {moves_label}")
        self._update_stats(zone_residents)

        target = self._target_positions(zone_residents, in_transit)
        if self._current_positions is None:
            self._current_positions = target
            self._draw_frame(target, in_transit)
        else:
            self._animate(self._current_positions, target, in_transit)

        self._updating_scale = True
        self.turn_scale.set(self.current_turn_index)
        self._updating_scale = False

    def _update_stats(self, zone_residents: Dict[str, List[str]]) -> None:
        total = len(self.result.turns_per_drone)
        delivered = len(zone_residents.get(self.graph.end_zone or "", []))
        turn_label = max(self.current_turn_index + 1, 0)
        self.stats_var.set(
            f"Turn {turn_label} / {self.result.total_turns}    "
            f"Delivered {delivered} / {total}"
        )

    def _target_positions(
        self, zone_residents: Dict[str, List[str]], in_transit: List[InTransitEntry]
    ) -> Dict[str, DronePosition]:
        positions: Dict[str, DronePosition] = {}
        for zone_name, labels in zone_residents.items():
            if zone_name not in self._positions:
                continue
            cx, cy = self._positions[zone_name]
            for offset, label in enumerate(labels):
                dx, dy = self._cluster_offset(offset)
                positions[label] = (cx + dx, cy + dy, False)

        for label, connection_name, destination in in_transit:
            origin = connection_name.rsplit(f"-{destination}", 1)[0]
            if origin not in self._positions or destination not in self._positions:
                continue
            x1, y1 = self._positions[origin]
            x2, y2 = self._positions[destination]
            positions[label] = ((x1 + x2) / 2, (y1 + y2) / 2, True)
        return positions

    def _animate(
        self,
        from_positions: Dict[str, DronePosition],
        to_positions: Dict[str, DronePosition],
        in_transit: List[InTransitEntry],
    ) -> None:
        self._animating = True

        def step(i: int) -> None:
            fraction = i / _ANIMATION_STEPS
            frame: Dict[str, DronePosition] = {}
            for label, (tx, ty, in_flight) in to_positions.items():
                if label in from_positions:
                    fx, fy, _ = from_positions[label]
                    x = fx + (tx - fx) * fraction
                    y = fy + (ty - fy) * fraction
                else:
                    x, y = tx, ty
                frame[label] = (x, y, in_flight)
            self._draw_frame(frame, in_transit)

            if i < _ANIMATION_STEPS:
                self.root.after(_ANIMATION_DELAY_MS, lambda: step(i + 1))
            else:
                self._current_positions = to_positions
                self._animating = False

        step(0)

    def _draw_frame(
        self, positions: Dict[str, DronePosition], in_transit: List[InTransitEntry]
    ) -> None:
        for item in self._drone_items:
            self.canvas.delete(item)
        self._drone_items = []

        # A drone here has not reached a zone yet: the subject models it as
        # occupying the *connection* for this turn (see VII.3: "the drone
        # occupies the connection during transit"). The connection it is
        # using is highlighted with a dashed, arrowed line -- and its
        # marker is a diamond, never a plain circle like a drone resting
        # in a zone -- so the midpoint never reads as a new, permanent stop.
        for _label, connection_name, destination in in_transit:
            origin = connection_name.rsplit(f"-{destination}", 1)[0]
            if origin not in self._positions or destination not in self._positions:
                continue
            x1, y1 = self._positions[origin]
            x2, y2 = self._positions[destination]
            highlight = self.canvas.create_line(
                x1, y1, x2, y2, fill=_ACCENT, width=3, dash=(6, 4), arrow=tk.LAST
            )
            self._drone_items.append(highlight)

        for label, (x, y, in_flight) in positions.items():
            self._draw_drone_marker(x, y, label, in_flight=in_flight)

    def _cluster_offset(self, index: int) -> Tuple[float, float]:
        # Small deterministic offsets so several drones sharing a zone
        # remain individually visible instead of stacking exactly.
        ring = [(0, 0), (16, -12), (-16, -12), (16, 12), (-16, 12), (0, -22), (0, 22)]
        return ring[index % len(ring)]

    def _draw_drone_marker(
        self, x: float, y: float, label: str, in_flight: bool = False
    ) -> None:
        if in_flight:
            # Diamond, not a circle: visually distinct from a drone resting
            # in a zone, so mid-connection never looks like a real stop.
            r = _DRONE_RADIUS + 3
            item = self.canvas.create_polygon(
                x, y - r, x + r, y, x, y + r, x - r, y,
                fill=_ACCENT, outline="white", width=1,
            )
            self._drone_items.append(item)
            suffix = " (in flight)"
        else:
            self._draw_drone_icon(x, y)
            suffix = ""
        text_item = self.canvas.create_text(
            x, y - _DRONE_RADIUS - 10, text=f"{label}{suffix}", font=("Segoe UI", 7, "bold")
        )
        self._drone_items.append(text_item)

    def _draw_drone_icon(self, x: float, y: float) -> None:
        # A tiny quadcopter glyph (body + four rotor arms) instead of a
        # plain dot, so drones read as drones rather than generic markers.
        arm = _DRONE_RADIUS + 6
        for ox, oy in ((-1, -1), (1, -1), (1, 1), (-1, 1)):
            tip_x = x + ox * arm * 0.7071
            tip_y = y + oy * arm * 0.7071
            arm_line = self.canvas.create_line(
                x, y, tip_x, tip_y, fill=_DRONE_COLOR, width=2
            )
            rotor = self.canvas.create_oval(
                tip_x - 3, tip_y - 3, tip_x + 3, tip_y + 3,
                fill="white", outline=_DRONE_COLOR, width=2,
            )
            self._drone_items.extend([arm_line, rotor])
        body = self.canvas.create_oval(
            x - _DRONE_RADIUS, y - _DRONE_RADIUS, x + _DRONE_RADIUS, y + _DRONE_RADIUS,
            fill=_DRONE_COLOR, outline="white", width=1,
        )
        self._drone_items.append(body)
