import sys


def main() -> None:
    args = sys.argv
    if len(args) != 2:
        print("Usage: ft_ancient_text.py <file>")
        return
    print("=== Cyber Archives Recovery ===")
    file = args[1]
    content = None
    try:
        content = open(file, "r")
        print(f"Accessing file '{file}'")
        print("---\n")
        print(content.read())
        print("---")
        content.close()
        print(f"File '{file}' closed.")
        content = None
    except FileNotFoundError:
        print(
            f"Error opening file '{file}': "
            f"[Errno 2] No such file or directory: '{file}'"
        )
    except PermissionError:
        print(
            f"Error opening file '{file}': "
            f"[Errno 13] Permission denied: '{file}'"
        )
    finally:
        if content is not None:
            content.close()
            print(f"File '{file}' closed.")


if __name__ == "__main__":
    main()
