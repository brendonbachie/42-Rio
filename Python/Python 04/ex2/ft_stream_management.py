import sys


def main():
    print("=== CYBER ARCHIVES - COMMUNICATION SYSTEM ===\n")

    ID = input("Input Stream active. Enter archivist ID: ")
    status_report = input("Input Stream active. Enter status report: ")
    print()
    msg = f"[STANDARD] Archive status from {ID}: {status_report}"
    print(msg, file=sys.stdout)
    print(
        "[ALERT] System diagnostic: Communication channels verified",
        file=sys.stderr,
    )
    print("[STANDARD] Data transmission complete\n", file=sys.stdout)
    print("Three-channel communication test successful.", file=sys.stdout)


if __name__ == "__main__":
    main()
