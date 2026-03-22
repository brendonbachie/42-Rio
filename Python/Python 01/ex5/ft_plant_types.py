class Plant:
    def __init__ (self, name: str, height: int, age: int):
        self.name = name
        self.height = height
        self.age = age

class Flower(Plant):
    def __init__ (self, name: str, height: int, age: int, color: str):
        super().__init__(name, height, age)
        self.color = color
    def blooming (self, bloom: str) -> str:
        self.bloom = bloom
        return f'{self.name}  is blooming {self.bloom}'

class Tree(Plant):
    def __init__ (self, name: str, height: int, age: int, trunk_diameter: int):
        super().__init__(name, height, age)
        self.trunk_diameter = trunk_diameter
    def produce_shade (self, shade: int) -> str:
        self.shade = shade
        return f'{self.name}  provides {self.shade} meters of shade'

class Vegetable(Plant):
    def __init__ (self, name: str, height: int, age: int, harvest_season: str):
        super().__init__(name, height, age)
        self.harvest_season = harvest_season
    def nutritional_value (self, nut: str) -> str:
        self.nut = nut
        return f'{self.name}  is rich in {self.nut}'


if __name__ == "__main__":
    flower = Flower("Rose", 30, 25, "Red")
    tree = Tree("Oak", 500, 365, 80)
    vegetable = Vegetable("Carrot", 20, 60, "Winter")

    print("=== Plant Information ===")
    print(f"{flower.name}: {flower.height}cm, {flower.age} days, color {flower.color}")
    print(f"{tree.name}: {tree.height}cm, {tree.age} days, trunk {tree.trunk_diameter}cm")
    print(f"{vegetable.name}: {vegetable.height}cm, {vegetable.age} days, harvest {vegetable.harvest_season}")

    print("\n=== Specific Behaviors ===")
    print(flower.blooming("beautifully"))
    print(tree.produce_shade(15))
    print(vegetable.nutritional_value("vitamin A"))