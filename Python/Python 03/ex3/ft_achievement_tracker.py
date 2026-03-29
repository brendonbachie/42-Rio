import random


def gen_player_achievements() -> set[str]:
    achievs = ['Crafting Genius', 'Strategist', 'World Savior',
               'Speed Runner', 'Survivor',
               'Master Explorer', 'Treasure Hunter', 'Unstoppable',
               'First Steps', 'Collector Supreme', 'Untouchable',
               'Sharp Mind', 'Boss Slayer']
    return set(random.sample(achievs, random.randint(0, len(achievs))))


def main() -> None:
    print("=== Achievement Tracker System ===\n")

    all_achievements = {'Crafting Genius', 'Strategist', 'World Savior',
                        'Speed Runner', 'Survivor',
                        'Master Explorer', 'Treasure Hunter', 'Unstoppable',
                        'First Steps', 'Collector Supreme', 'Untouchable',
                        'Sharp Mind', 'Boss Slayer'}

    names = ['Alice', 'Bob', 'Charlie', 'Dylan']
    players = []
    for name in names:
        players += [(name, gen_player_achievements())]

    for name, player in players:
        print(f"Player {name}: {player}")

    all_distict_achievs: set[str] = set()

    for _, achievements in players:
        all_distict_achievs = all_distict_achievs.union(achievements)
    print(f"\nAll distinct achievements: {all_distict_achievs}")

    common_achievements: set[str] = set()
    for _, achievements in players:
        common_achievements = all_achievements.intersection(achievements)
    print(f"\nCommon achievements: {common_achievements}\n")

    for name, achievements in players:
        difference = achievements
        for player2, other_achievements in players:
            if player2 != name:
                difference = difference.difference(other_achievements)
        print(f"Only {name} has: {difference}")

    print()

    for name, achievement in players:
        print(f"{name} is missing: {all_achievements.difference(achievement)}")


if __name__ == "__main__":
    main()
