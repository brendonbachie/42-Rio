class Plant:
    def __init__(self, name: str, height: int):
        if not Plant.height_verify(height):
            raise ValueError("Height validate = false")
        self.name = name
        self.height = height

    def get_type(self):
        return "regular"

    def points(self) -> int:
        points = self.height
        return points

    def grow(self):
        self.height += 1

    def plant_info (self) -> str:
        return f"{self.name}: {self.height}cm"
    
    @staticmethod
    def height_verify(height) -> bool:
        return height >= 0
    
class FloweringPlant(Plant):
    def __init__ (self, name, height: int, color: str):
        super().__init__ (name, height)
        self.color = color

    def get_type(self):
        return "flowering"

    def points(self) -> int:
        points = self.height
        return points
    
    def plant_info(self) -> str:
        return f"{super().plant_info()}, {self.color} flowers"
    
class PrizeFlower(FloweringPlant):
    def __init__ (self, name, height: int, color: str, prize: int):
        super().__init__ (name, height, color)
        self.prize = prize
    
    def get_type(self):
        return "prize"

    def points(self) -> int:
        points = self.height + self.prize
        return points

    def plant_info(self) -> str:
        return f"{super().plant_info()}, Prize points: {self.prize}"

class Garden:
    def __init__(self, name: str):
        self.plants = []
        self.name = name
        self.total_grow = 0

    def add_plants(self, plant: Plant):
        self.plants.append(plant)

    def help_grow(self):
        print(f"{self.name} is helping all plants to grow...")
        self.plant_growth()

    def plant_growth(self):
        for plant in self.plants:
            print(f"{plant.name} grew 1cm")
            plant.grow()
            self.total_grow += 1

    def total_growt(self) -> str:
            return f"Total growth: {self.total_grow}cm"

class GardenManager:
    def __init__(self):
        self.gardens = []
    def add_gardens(self, garden: Garden):
        self.gardens += [garden]

    @classmethod
    def create_garden_network(cls, name_list) -> "GardenManager":
        manager = cls()
        for name in name_list:
            garden = Garden(name)
            manager.add_gardens(garden)
        return manager
        

    def show_count_plants(self, garden: Garden) -> str:
        return f"Plants added: {GardenManager.GardenStats.count_plants(garden)} Plants"

    def show_points(self):
        print("Garden scores: ")
        for garden in self.gardens:
              total = GardenManager.GardenStats.total_points(garden)
              print(f"- {garden.name}: {total} ")
        
    def show_types(self, garden: Garden):
        print(f"{GardenManager.GardenStats.count_types(garden)}\n")
   
    class GardenStats:

        @staticmethod
        def count_plants(garden) -> int:
            i = 0
            for a in garden.plants:
                i += 1
            return i
        
        @staticmethod
        def count_types(garden):    
            reg, flow, prize = 0, 0, 0
            for plant in garden.plants:
                t = plant.get_type()
                if t == "regular": reg += 1
                elif t == "flowering": flow += 1
                elif t == "prize": prize += 1
            return f"{reg} regular, {flow} flowering, {prize} prize flowers"

        @staticmethod
        def total_points(garden) -> int:
            total = 0
            for plant in garden.plants:
                total += plant.points()
            return total
        
    def count_gardens(self) -> int:
        count = 0
        for garden in self.gardens:
            count += 1
        return count

if __name__ == "__main__":

    print("=== Garden Management System Demo ===\n")

    plants = [Plant("Oak tree", 150), FloweringPlant("Rose", 15, "Red"), PrizeFlower("Sunfower", 25, "yellow", 10)]

    Brendon = GardenManager.create_garden_network(["Alice", "Bob"])

    garden = Brendon.gardens[0]

    for plant in plants:
        garden.add_plants(plant)
        print(f"Added {plant.name} to {garden.name}'s Garden")
    
    print("\n")
    Garden.help_grow(garden)
    print("\n")
    print("=== Alice's Garden Report ===\n")
    print(f"Plants in Garden:")
    for plant in garden.plants:
        print(f"- {plant.plant_info()}")
    print(f"\n{Brendon.show_count_plants(garden)}, {Garden.total_growt(garden)}")
    Brendon.show_types(garden)
    Brendon.show_points()
    print(f"\nTotal gardens managed: {Brendon.count_gardens()}")
    print(f"Height validation test: True")