import sys


def main() -> None:
    args = sys.argv
    if len(args) != 2:
        print("Usage: ft_ancient_text.py <file>")
        return
    print("=== Cyber Archives Recovery ===")
    file_name = args[1]
    content = None
    file_to_change = None
    try:
        content = open(file_name, "r")
        print(f"Accessing file '{file_name}'")
        print("---\n")
        file_to_change = content.read()
        print(file_to_change)
        print("---")
        content.close()
        print(f"File '{file_name}' closed.")
        content = None
        print("Transform data:")
        print("---\n")
        for line in file_to_change.splitlines():
            print(f"{line}#")
        print("---")
    except FileNotFoundError:
        print(
            f"[STDERR] Error opening file '{file_name}': "
            f"[Errno 2] No such file or directory: '{file_name}'",
            file=sys.stderr)
        return
    except PermissionError:
        print(
            f"[STDERR] Error opening file '{file_name}': "
            f"[Errno 13] Permission denied: '{file_name}'",
            file=sys.stderr)
        return
    except Exception as e:
        print(
            f"[STDERR] Error opening file '{file_name}': {e}",
            file=sys.stderr)
        return
    finally:
        if content is not None:
            content.close()
            print(f"File '{file_name}' closed.\n")
    print("Enter new file (or empty): ", end="", flush=True)
    new_file = sys.stdin.readline().strip()
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
            err_msg = (
                f"[STDERR] Error saving file '{new_file}': {e}"
            )
            print(err_msg, file=sys.stderr)
            print("Data not saved.")
        finally:
            if new is not None:
                new.close()
                print(f"File '{new_file}' closed.")
    else:
        print("Not saving data")


if __name__ == "__main__":
    main()
