def check_temperature(temp_str) -> None:

    try:	
        temp = int(temp_str)
    except:
        print(f"Error: {temp_str} is not a valid number\n")
        return
    if temp > 40:
        print(f"Error: {temp}°C is too hot for plants (max 40°C)\n")
    elif temp < 0:
        print(f"Error: {temp}°C is too cold for plants (min 0°C)\n")
    else:
        print(f"Temperature {temp}°C is perfect for plants!\n")
    
def test_temperature_input() -> None:
    temp = 25
    print(f"Testing temperature: {temp}")
    check_temperature(temp)
    temp = 100
    print(f"Testing temperature: {temp}")
    check_temperature(temp)
    temp = -50
    print(f"Testing temperature: {temp}")
    check_temperature(temp)
    temp = 'abc'
    print(f"Testing temperature: {temp}")
    check_temperature(temp)

if __name__ == "__main__":
    test_temperature_input()