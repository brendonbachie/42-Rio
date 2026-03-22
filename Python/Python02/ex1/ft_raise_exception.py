def input_temperature(temp_str) -> int:
    temp = int(temp_str)
    if temp < 0:
        raise ValueError(f"{temp}°C is too cold for plants (min 0°C)")
    elif temp > 40:
        raise ValueError(f"{temp}°C is too hot for plants (max 40°C)")
    return temp


def test_temperature_input() -> None:
    print("===Garden Temperature ===\n")
    temp = [25, 96, 'abc', 15, '30C', 0]
    for t in temp:
        try:
            print(f"Input data is '{t}'")
            t = input_temperature(t)
            print(f"Temperature is now {t}°C\n")
        except ValueError as e:
            print(f"Caught input_temperature error: {e}\n")

    print("All tests completed - program didn't crash!")


if __name__ == "__main__":
    test_temperature_input()
