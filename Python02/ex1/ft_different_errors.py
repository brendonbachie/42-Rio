def garden_operations():
	
    print("Testing ValueError...")
    try:
        int('abc')
    except:
        print("Caught ValueError: invalid literal for int()\n")

    print("Testing ZeroDivisionError...")
    try:
        i = 10/0
    except:
        print("Caught ZeroDivisionError: division by zero\n")

    print("Testing FileNotFoundError...")
    try:
        f = open('missing.txt')
    except:
        print(f"Caught FileNotFoundError: No such file {f}\n")

    print("Testing KeyError...")
    try:
        d = {}
        print(f"{d[5]}")
    except KeyError as e:
        print(f"Caught KeyError: '{e}'\n")
    
    try:
        10/0
        int('abc')
    except:
        print("Caught an error, but program continues!")