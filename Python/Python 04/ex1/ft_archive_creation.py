import sys


def main() -> None:
    args = sys.argv
    if len(args) != 2:
        print("Usage: ft_ancient_text.py <file>")
        return
    print("=== Cyber Archives Recovery & Preservation ===")
    file = args[1]
    content = None
    file_to_change = None
    try:
        content = open(file, "r")
        print(f"Accessing file '{file}'")
        print("---\n")
        file_to_change = content.read()
        print(file_to_change)
        print("---")
        content.close()
        print(f"File '{file}' closed.")
        content = None
    except FileNotFoundError:
        print(
            f"Error opening file '{file}': "
            f"[Errno 2] No such file or directory: '{file}'"
        )
        return
    except PermissionError:
        print(
            f"Error opening file '{file}': "
            f"[Errno 13] Permission denied: '{file}'"
        )
        return
    finally:
        if content is not None:
            content.close()
            print(f"File '{file}' closed.")
    print("Transform data:")
    print("---\n")
    for line in file_to_change.splitlines():
        print(f"{line}#")
    print("---")
    new_file = input("Enter new file (or empty): ")
    new = None
    if new_file:
        print(f"Saving data to '{new_file}'")
        try:
            new = open(new_file, "w")
            for line in file_to_change.splitlines():
                new.write(f"{line}#\n")
            new.close()
            print(f"Data saved in file '{new_file}'")
            new = None
        except Exception as e:
            print(f"Error saving file '{new_file}': {e}")
        finally:
            if new is not None:
                new.close()
                print(f"File '{new_file}' closed.")
    else:
        print("Not saving data")


if __name__ == "__main__":
    main()
