from ex0 import FlameFactory, AquaFactory
from ex1 import HealingCreatureFactory, TransformCreatureFactory
from ex2 import NormalStrategy, DefensiveStrategy, AggressiveStrategy


def battle(opponents: list) -> None:
    print("*** Tournament ***")
    print(f"{len(opponents)} opponents involved")

    for i, opponent1 in enumerate(opponents):
        for opponent2 in opponents[i+1:]:
            factory1, strategy1 = opponent1
            factory2, strategy2 = opponent2
            creature1 = factory1.create_base()
            creature2 = factory2.create_base()
            print("* Battle *")
            print(creature1.describe())
            print(" vs.")
            print(creature2.describe())
            print(" now fight!")
            try:
                print(strategy1.act(creature1))
                print(strategy2.act(creature2))
            except ValueError as e:
                print(f"Battle error, aborting tournament: {e}")
                return


if __name__ == "__main__":
    print("Tournament 0 (basic)")
    opponents = [
        (FlameFactory(), NormalStrategy()),
        (HealingCreatureFactory(), DefensiveStrategy()),
    ]
    battle(opponents)

    print("\nTournament 1 (error)")
    opponents = [
        (FlameFactory(), AggressiveStrategy()),
        (HealingCreatureFactory(), DefensiveStrategy()),
    ]
    battle(opponents)

    print("\nTournament 2 (multiple)")
    opponents = [
        (AquaFactory(), NormalStrategy()),
        (HealingCreatureFactory(), DefensiveStrategy()),
        (TransformCreatureFactory(), AggressiveStrategy()),
    ]
    battle(opponents)
