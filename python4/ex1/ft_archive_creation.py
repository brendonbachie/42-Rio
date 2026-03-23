def main() -> None:
    print("=== CYBER ARCHIVES - PRESERVATION SYSTEM ===")
    print("Initializing new storage unit: new_discovery.txt")
    try:
        file = open("new_discovery.txt", "w")
        print("Storage unit created successfully...")
        print("Inscribing preservation data...")
        file.write("[ENTRY 001] New quantum algorithm discovered\n")
        file.write("[ENTRY 002] Efficiency increased by 347%\n")
        file.write("[ENTRY 003] Archived by Data Archivist trainee\n")
        print("Data inscription complete. Storage unit sealed.")
    except Exception as e:
        print(f"ERROR: {e}")
    finally:
        file.close()
    print("Archive 'new_discovery.txt' ready for long-term preservation.")


if __name__ == "__main__":
    main()
