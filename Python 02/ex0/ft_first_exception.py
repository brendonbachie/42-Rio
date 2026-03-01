def check_temperature(temp_str):
    print (f"Testing temperature: {temp_str}")
    try:
        temp = int(temp_str)
    except:
        print (f"Error: '{temp_str}' is not a valid number")
        return

    if temp > 40:
        print (f"Error: {temp}°C is too hot for plants (max 40°C)")
        return
    elif temp < 0:
        print (f"Error: {temp}°C is too cold for plants (min 0°C)")
        return
    else:
        print (f"Temperature {temp}°C is perfect for plants!")
        return temp

def test_temperature_input():
    check_temperature("35")
    check_temperature("40")
    check_temperature("-10")
    check_temperature("12abc")

if __name__ == "__main__":
    test_temperature_input()