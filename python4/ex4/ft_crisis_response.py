
def main() -> None:
    print("=== CYBER ARCHIVES - CRISIS RESPONSE SYSTEM ===\n")

    try:
        print("CRISIS ALERT: Attempting access to 'lost_archive.txt'...")
        with open("lost_archive.txt", "r") as file:
            content = file.read()
            print(f"SUCCESS: Archive recovered - {content}")
            print("STATUS: Normal operations resumed")
    except FileNotFoundError:
        print("RESPONSE: Archive not found in storage matrix")
        print("STATUS: Crisis handled, system stable")
    except PermissionError:
        print("RESPONSE: Security protocols deny access")
        print("STATUS: Crisis handled, system stable")

    print()

    try:
        print("Attempting access to 'classified_vault.txt'...")
        with open("../classified_vault.txt", "r") as file:
            content = file.read()
            print(f"SUCCESS: Archive recovered - {content}")
            print("STATUS: Normal operations resumed")
    except FileNotFoundError:
        print("RESPONSE: Archive not found in storage matrix")
        print("STATUS: Crisis handled, system stable")
    except PermissionError:
        print("RESPONSE: Security protocols deny access")
        print("STATUS: Crisis handled, system stable")

    print()

    try:
        print("Attempting access to 'standard_archive.txt'...")
        with open("../standard_archive.txt", "r") as file:
            content = file.read()
            print(f"SUCCESS: Archive recovered - ''{content}''")
            print("STATUS: Normal operations resumed")
    except Exception as e:
        print(f"ERROR: {e}")

    print("\nAll crisis scenarios handled successfully. Archives secure")


if __name__ == "__main__":
    main()
