def input_temperature(temp_str) -> int:
    return int(temp_str)


def test_temperature_input() -> None:
    print("===Garden Temperature ===\n")
    temp = [25, 96, 'abc', 15, '30C']
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
