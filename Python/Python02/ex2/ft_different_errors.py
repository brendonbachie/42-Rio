def garden_operations(operation_number: int) -> None:
    if operation_number == 0:
        int('abc')
    elif operation_number == 1:
        10/0
    elif operation_number == 2:
        open('missing.txt')
    elif operation_number == 3:
        'str' + 5
    else:
        print("Operation completed successfully.")


def test_error_types() -> None:
    print("=== Garden Error Types Demo ===")
    for operation_number in range(5):
        print(f"Testing operation {operation_number}...")
        try:
            garden_operations(operation_number)
        except ValueError as e:
            print(f"Caught a ValueError: {e}")
        except ZeroDivisionError as e:
            print(f"Caught a ZeroDivisionError: {e}")
        except FileNotFoundError as e:
            print(f"Caught a FileNotFoundError: {e}")
        except TypeError as e:
            print(f"Caught a TypeError: {e}")

    print("\nAll error types tested successfully!")


if __name__ == "__main__":
    test_error_types()
