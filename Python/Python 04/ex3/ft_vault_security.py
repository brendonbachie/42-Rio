import typing


def secure_archive(file_name: str, action: str) -> typing.Tuple[bool, str]:
    try:
        if action == "read":
            with open(file_name, "r") as f:
                content = f.read()
                return True, content
        elif action == "write":
            with open(file_name, "w") as f:
                f.write("Secure data")
                return True, "Content successfully written to file"
        else:
            return False, "Invalid action"
    except Exception as e:
        return False, str(e)


def main() -> None:
    print("=== Cyber Archives Security ===")
    print("Using 'secure_archive' to read from a nonexistent file:")
    result = secure_archive("/not/existing/file", "read")
    print(result)
    print("Using 'secure_archive' to read from an inaccessible file:")
    result = secure_archive("/etc/master.passwd", "read")
    print(result)
    print("Using 'secure_archive' to read from a regular file:")
    result = secure_archive("ancient_fragment.txt", "read")
    print(result)
    if result[0]:
        print(
            "Using 'secure_archive' to write previous content to a new "
            "file:"
        )
        result = secure_archive("new_secure_file.txt", "write")
        print(result)


if __name__ == "__main__":
    main()
