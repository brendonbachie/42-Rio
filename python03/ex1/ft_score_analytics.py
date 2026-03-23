import sys


def main() -> None:
    print("== Player Score Analytics ===")
    scores = sys.argv[1:]
    player_score = []
    if not scores:
        print(
            "No scores provided. Usage: python3 ft_score_analytics.py "
            "<score1> <score2> ..."
        )
        return
    for score in scores:
        if not score.isdigit():
            print(f"Invalid parameter: '{score}'")
    try:
        for score in scores:
            player_score += [int(score)]
    except ValueError:
        print(
            "No scores provided. Usage: python3 ft_score_analytics.py "
            "<score1> <score2> ..."
        )
        return
    print(f"Scores processed: {player_score}")
    print(f"Total players: {len(player_score)}")
    print(f"Total score: {sum(player_score)}")
    print(f"Average score: {sum(player_score) / len(player_score)}")
    print(f"High score: {max(player_score)}")
    print(f"Low score: {min(player_score)}")
    print(f"Score range: {max(player_score) - min(player_score)}")


if __name__ == "__main__":
    main()
