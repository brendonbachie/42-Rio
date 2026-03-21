class Plant():
    def __init__(self, name: str, time_watered: int):
        self.name = name
        self.time_watered = time_watered


class Garden():
    def __init__(self, tank_level: int):
        self.plants = []
        self.tank_level = tank_level


class GardenError(Exception):
    pass


class PlantError(GardenError):
    pass


class WaterError(GardenError):
    pass


def demo_custom_errors():

    my_garden = Garden(4)
    plant = Plant("Rose", 6)
    my_garden.plants.append(plant)

    print("=== Custom Garden Errors Demo ===\n")

    print("Testing PlantError...")
    try:
        if my_garden.plants[0].time_watered > 5:
            raise PlantError
    except PlantError:
        print(f"Caught PlantError: The {plant.name} plant is wilting!")

    print("\nTesting WaterError...")
    try:
        if my_garden.tank_level < 5:
            raise WaterError
    except WaterError:
        print("Caught WaterError: Not enough water in the tank!")

    print("\nTesting catching all garden errors...")
    try:
        if my_garden.plants[0].time_watered > 5:
            raise PlantError
    except GardenError:
        print(f"Caught a GardenError: The {plant.name} plant is wilting!")

    try:
        if my_garden.tank_level < 5:
            raise WaterError
    except GardenError:
        print("Caught a GardenError: Not enough water in the tank!")

    print("\nAll custom error types work correctly!")


if __name__ == "__main__":
    demo_custom_errors()
