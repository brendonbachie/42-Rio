def main() -> None:
    print("=== CYBER ARCHIVES - DATA RECOVERY SYSTEM ===\n")
    try:
        file = open("ancient_fragment.txt", "r")
        print(f"Accessing Storage Vault: {file.name}")
        print("Connection established...\n")
        print("RECOVERED DATA:")
        for line in file:
            print(line.strip())
        print("\nData recovery complete. Storage unit disconnected.")
        file.close()
    except FileNotFoundError:
        print("ERROR: Storage vault not found. Run data generator first.")


if __name__ == "__main__":
    main()
