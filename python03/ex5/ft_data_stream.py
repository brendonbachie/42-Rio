from typing import Generator
import random


def gen_event() -> Generator[tuple, None, None]:
    names = ['Alice', 'Bob', 'Dylan', 'Charlie', ]
    actions = ['run', 'eat', 'sleep',
               'grab', 'move', 'climb', 'swim', 'release']
    while True:
        yield (random.choice(names), random.choice(actions))


def consume_event(events: list) -> Generator[tuple, None, None]:
    new_list = events

    while new_list:
        result = (random.choice(new_list))
        print(f"Got event from list: {result}")
        temp = new_list
        new_list = []
        for tuple in temp:
            if tuple != result:
                new_list += [tuple]
        print(f"Remains in list: {new_list}")
        yield result


def main():
    print("=== Game Data Stream Processor ===")
    events = []
    result = gen_event()
    for i in range(0, 1000):
        name, action = next(result)
        print(f"Event {i}: Player {name} did action {action}")

    new_list = gen_event()
    for _ in range(10):
        name, action = next(new_list)
        events += [(name, action)]
    print(f"Built list of 10 events: {events}")

    for _ in consume_event(events):
        pass


if __name__ == "__main__":
    main()
