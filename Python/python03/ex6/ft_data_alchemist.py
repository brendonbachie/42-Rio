import random


def main() -> None:
    print("=== Game Data Alchemist ===\n")

    initial_list = ['Alice', 'bob', 'Charlie',
                    'dylan', 'Emma', 'Gregory', 'john', 'kevin', 'Liam']

    only_cap = [string for string in initial_list if string[0].isupper()]

    all_cap = [string.capitalize() for string in initial_list]

    scores = {name: random.randint(0, 1000) for name in all_cap}

    average = sum([scores[value] for value in scores]) / len(all_cap)

    high = {name: scores[name] for name in scores if scores[name] > average}

    print(f"Initial list of players: {initial_list}")
    print(f"New list with all names capitalized: {all_cap}")
    print(f"New list of capitalized names only: {only_cap}\n")
    print(f"Score dict: {scores}")
    print(f"Score average is {round(average, 2)}")
    print(f"High scores: {high}")


if __name__ == "__main__":
    main()
