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
        	return f"{self.name}: {self.height}cm, {self.age} days old"

if __name__ == "__main__":
	rose = Plant("Rose", 25, 30)
	print("=== Day 1 ===")
	print(rose.get_info())
	for _ in range(6):
		rose.grow()
		rose.age()
	print("=== Day 7 ===")
	print(rose.get_info())
	print("Growth this week: +6cm")
