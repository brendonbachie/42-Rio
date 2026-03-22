class Plant:
    def __init__ (self, name: str, height: int, age: int):
        self.name = name
        self.__height = 0
        self.__age = 0
        self.set_height(height)
        self.set_age(age)
    def get_info(self) -> str:
            return f"{self.name} ({self.__height}cm, {self.__age} days)"
    def get_age(self) -> int:
         return self.__age
    def get_height(self) -> int:
            return self.__height
    def set_height(self, height: int):
        if (height >= 0):
              print(f"Height updated: {height}cm [OK]")
              self.__height = height
        else:
              print(f"Invalid operation attempted: height {height}cm [REJECTED]")
              print("Security: Negative height rejected")
    def set_age(self, age: int):
        if (age >= 0):
              self.__age = age
              print(f"Age updated: {age} days [OK]")
        else:
              print(f"Invalid operation attempted: age {age} days [REJECTED]")
              print("Security: Negative age rejected")

if __name__ == "__main__":
    plants = [Plant("Rose", 25, 30), 
    Plant("Oak", 200, 365), 
    Plant("Cactus", 5, 90), 
    Plant("Sunflower", 80, 45),
    Plant("Fern", 15, 120)]
    for plant in plants:
        print(f"{plant.name}: {plant.get_height()}cm, {plant.get_age()} days")