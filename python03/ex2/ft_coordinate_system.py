import math


def get_player_pos():

    while True:
        coord = input("Enter new coordinates as floats in format 'x,y,z': ")
        try:
            x, y, z = coord.split(",")
            x = round(float(x.strip()), 2)
            y = round(float(y.strip()), 2)
            z = round(float(z.strip()), 2)
            return x, y, z
        except ValueError:
            print("Invalid syntax")
            continue


def main():
    print("Get a first set of coordinates")
    x1, y1, z1 = get_player_pos()
    print(f"Got a first tuple: ({x1}, {y1}, {z1})")
    print(f"It includes: X={x1}, Y={y1}, Z={z1}")
    x2, y2, z2 = 0.0, 0.0, 0.0
    distance_to_center = math.sqrt((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)
    print(f"Distance to center: {distance_to_center:.4f}\n")

    print("Get a second set of coordinates")
    while True:
        coord = input("Enter new coordinates as floats in format 'x,y,z': ")
        try:
            x2, y2, z2 = coord.split(",")
            x2 = round(float(x2.strip()), 2)
        except ValueError as e:
            print(f"Error on parameter '{x2}': {e}")
            continue
        try:
            y2 = round(float(y2.strip()), 2)
        except ValueError as e:
            print(f"Error on parameter '{y2}': {e}")
            continue
        try:
            z2 = round(float(z2.strip()), 2)
            break
        except ValueError as e:
            print(f"Error on parameter '{z2}': {e}")
            continue
    distance_to_center = math.sqrt((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)
    print(
        "Distance between the 2 sets of coordinates: "
        f"{distance_to_center:.4f}"
    )


if __name__ == "__main__":
    main()
