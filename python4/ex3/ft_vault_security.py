def main() -> None:
    print("=== CYBER ARCHIVES - VAULT SECURITY SYSTEM ===\n")

    print("Initiating secure vault access...")
    try:
        with open("../classified_data.txt", "r") as file:
            print("Vault connection established with failsafe protocols\n")
            print("SECURE EXTRACTION:")
            content = file.read()
            print(f"{content}")
        with open("../security_protocols.txt", "r") as file:
            print("\nSECURE PRESERVATION:")
            print(f"{file.read()}")
    except Exception as e:
        print(f"ERROR: {e}")
        
    print("Vault automatically sealed upon completion\n")
    print("All vault operations completed with maximum security.")


if __name__ == "__main__":
    main()
