import os
from dotenv import load_dotenv


def load_config() -> dict[str, str | None]:
    load_dotenv()
    matrix_mode: str | None = os.environ.get('MATRIX_MODE')
    database_url: str | None = os.environ.get('DATABASE_URL')
    api_key: str | None = os.environ.get('API_KEY')
    log_level: str | None = os.environ.get('LOG_LEVEL')
    zion_endpoint: str | None = os.environ.get('ZION_ENDPOINT')
    return {
        "matrix_mode": matrix_mode,
        "database_url": database_url,
        "api_key": api_key,
        "log_level": log_level,
        "zion_endpoint": zion_endpoint
    }


def main() -> None:
    print("\nORACLE STATUS: Reading the Matrix...\n")
    config: dict[str, str | None] = load_config()
    print("Configuration loaded:")
    print(f"Mode: {config['matrix_mode']}")
    if config['matrix_mode'] == 'development':
        print(f"Database: {'Connected to local instance'
                           if config['database_url'] else 'Not connected'}")
        print(f"API Access: {'Authenticated'
                             if config['api_key'] else 'Not authenticated'}")
        print(f"Log Level: {config['log_level']}")
        print(f"Zion Network: {'Online'
                               if config['zion_endpoint'] else 'Offline'}\n")
    else:
        print(f"Database: {'Connected'
                           if config['database_url'] else 'Not connected'}")
        print(f"API Access: {'Authenticated'
                             if config['api_key'] else 'Not authenticated'}")
        print(f"Log Level: {config['log_level']}")
        print(f"Zion Network: {'Online'
                               if config['zion_endpoint'] else 'Offline'}\n")

    print("Environment security check:")

    if os.path.exists('.env'):
        if config['api_key'] and 'SECRET' in config['api_key'].upper():
            print("[WARNING] Hardcoded secrets detected in API_KEY")
        else:
            print("[OK] No hardcoded secrets detected")
        if (config['matrix_mode'] and config['database_url']
                and config['api_key'] and config['log_level']
                and config['zion_endpoint']):
            print("[OK] .env file properly configured")
        else:
            print("[WARNING] .env file missing required configurations")
        print("[OK] Production overrides available")
    else:
        print("[WARNING] .env file not found")
        print("[WARNING] Production overrides not available")

    print("The Oracle sees all configurations.")


if __name__ == "__main__":
    main()
