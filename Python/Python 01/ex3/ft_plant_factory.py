class Plant:
	def __init__ (self, name: str, height: int, age: int):
		self.name = name
		self.height = height
		self.age = age
	def __grow__ (self):
		self.height += 1
	def __age__ (self):
		self.age += 1
	def get_info(self) -> str:
        	return f"{self.name} ({self.height}cm, {self.age} days)"

if __name__ == "__main__":
    print("=== Plant Factory Output ===")
    plants = [Plant("Rose", 25, 30), 
    Plant("Oak", 200, 365), 
    Plant("Cactus", 5, 90), 
    Plant("Sunflower", 80, 45), 
    Plant("Fern", 15, 120)]
    for plant in plants:
        print(f"Created: {plant.get_info()}")
    print(f"\nTotal plants created: {len(plants)}")