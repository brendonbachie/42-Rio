def garden_operations():
    print ("Testing ValueError...")
    try:
        int('abc')
    except ValueError:
        print("Caught ValueError: invalid literal for int()\n")
    print ("Testing ZeroDivisionError...")
    try:
        1 / 0
    except ZeroDivisionError:
        print ("Caught ZeroDivisionError: division by zero\n")
    print("Testing FileNotFoundError...")
    try:
        f = open("missing.txt", "r")
        f.close()
    except FileNotFoundError:
        print("Caught FileNotFoundError: No such file 'missing.txt'\n")
    print ("Testing KeyError...")
    try:
        d = {}
        print(d[9])
    except KeyError as e:
        print(f"Caught KeyError: {e}\n")
    print("Testing multiple errors together...")
    try:
        int('abc')
        1 / 0
    except:
        print ("Caught an error, but program continues!\n")
    
def test_error_types():
    print("=== Garden Error Types Demo ===\n")
    garden_operations()
    print("All error types tested successfully")


if __name__ == "__main__":
    test_error_types()