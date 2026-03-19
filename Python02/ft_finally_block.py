class PlantError(Exception):
    pass

def water_plant(plant_name):
    if plant_name[0].isupper():
        print(f"Watering {plant_name}: [OK]")
    else:
        raise PlantError(f"Caught PlantError: Invalid plant name to water: '{plant_name}'")
        
def test_watering_system():
    plants = ["Rose", "Daisy", "Tulip", "Sunflower"]
    try:
        print("Testing valid plants...")
        try:
            print("Opening watering system")
            for plant in plants:
                water_plant(plant)
        except PlantError:
            print(".. ending tests and returning to main")
        finally:
            print("Closing watering system\n")
    
        print("Testing invalid plant...")
        plants = ["Rose", "daisy", "Tulip", "Sunflower"]
        try:
            print("Opening watering system")
            for plant in plants:
                water_plant(plant)
        except PlantError as e:
            print(e)
            print(".. ending tests and returning to main")
            return
        finally:
            print("Closing watering system\n")
    finally:
        print("Cleanup always happens, even with errors!")

if __name__ == "__main__":
    test_watering_system()
