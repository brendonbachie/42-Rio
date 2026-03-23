import math


def get_player_pos() -> tuple[float, float, float]:

    while True:
        coord = input("Enter new coordinates as floats in format 'x,y,z': ")
        if len(coord.split(",")) != 3:
            print("invalid syntax")
            continue
        try:
            x, y, z = coord.split(",")
            x = round(float(x.strip()), 2)
        except ValueError as e:
            print(f"Error on parameter '{x}': {e}")
            continue
        try:
            y = round(float(y.strip()), 2)
        except ValueError as e:
            print(f"Error on parameter '{y}': {e}")
            continue
        try:
            z = round(float(z.strip()), 2)
        except ValueError as e:
            print(f"Error on parameter '{z}': {e}")
            continue
        return x, y, z


def main() -> None:
    print("=== Game Coordinate System ===\n")
    print("Get a first set of coordinates")
    x1, y1, z1 = get_player_pos()
    print(f"Got a first tuple: ({x1}, {y1}, {z1})")
    print(f"It includes: X={x1}, Y={y1}, Z={z1}")
    x2, y2, z2 = 0.0, 0.0, 0.0
    distance_to_center = math.sqrt((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)
    print(f"Distance to center: {distance_to_center:.4f}\n")

    print("Get a second set of coordinates")
    x2, y2, z2 = get_player_pos()
    distance_two_players = math.sqrt((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)
    print(
        "Distance between the 2 sets of coordinates: "
        f"{distance_two_players:.4f}"
    )


if __name__ == "__main__":
    main()
