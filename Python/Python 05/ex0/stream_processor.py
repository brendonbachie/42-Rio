from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):
    def __init__(self):
        self._storage = []
        self._counter = 0

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    @abstractmethod
    def ingest(self, data: Any) -> None:
        pass

    def output(self) -> tuple[int, str]:
        if not self._storage:
            raise ValueError("No data to output")
        return self._storage.pop(0)


class NumericProcessor(DataProcessor):
    def __init__(self):
        super().__init__()

    def validate(self, data: Any) -> bool:
        if isinstance(data, (int, float)):
            return True
        if isinstance(data, list):
            return all(isinstance(item, (int, float)) for item in data)
        return False

    def ingest(self, data: Any) -> None:
        if not self.validate(data):
            raise ValueError("Improper numeric data")
        if isinstance(data, (int, float)):
            item = (self._counter, str(data))
            self._storage.append(item)
            self._counter += 1
        elif isinstance(data, list):
            for item in data:
                item = (self._counter, str(item))
                self._storage.append(item)
                self._counter += 1


class TextProcessor(DataProcessor):
    def __init__(self):
        super().__init__()

    def validate(self, data: Any) -> bool:
        if isinstance(data, str):
            return True
        if isinstance(data, list):
            if all(isinstance(item, str) for item in data):
                return True
        return False

    def ingest(self, data: Any) -> None:
        if not self.validate(data):
            raise ValueError("Improper text data")
        if isinstance(data, str):
            item = (self._counter, data)
            self._storage.append(item)
            self._counter += 1
        elif isinstance(data, list):
            for item in data:
                item = (self._counter, item)
                self._storage.append(item)
                self._counter += 1


class LogProcessor(DataProcessor):
    def __init__(self):
        super().__init__()

    def validate(self, data: Any) -> bool:
        if isinstance(data, dict):
            if 'log_level' in data and 'log_message' in data:
                return True
        if isinstance(data, list):
            return all(isinstance(item, dict) and 'log_level' in item and 'log_message' in item for item in data)
        return False

    def ingest(self, data: Any) -> None:
        if not self.validate(data):
            raise ValueError("Improper log data")
        if isinstance(data, dict):
            item = (self._counter, f"{data['log_level']}: {data['log_message']}")
            self._storage.append(item)
            self._counter += 1
        elif isinstance(data, list):
            for item in data:
                item = (self._counter, f"{item['log_level']}: {item['log_message']}")
                self._storage.append(item)
                self._counter += 1


def main_num() -> None:
    print("Testing NumericProcessor...")

    number_list = [1, 2, 3, 4, 5]
    num_processor = NumericProcessor()

    print(f" Trying to validate input '42': {num_processor.validate(42)}")
    print(f" Trying to validate input 'Hello': {num_processor.validate('Hello')}")
    print(f" Test invalid ingestion of string 'foo'without prior validation:")
    try:
        num_processor.ingest("foo")
    except ValueError as e:
        print(f" Got exception: {e}")

    print(f"Processing data: {number_list}")
    for item in number_list:
        if num_processor.validate(item):
            num_processor.ingest(item)

    print("Extracting 3 values")
    for _ in range(3):
        try:
            index, value = num_processor.output()
            print(f" Numeric value {index}: {value}")
        except ValueError as e:
            print(f" Got exception: {e}")


def main_text() -> None:
    print("Testing TextProcessor...")

    text_list = ["Hello", "Nexus", "World"]
    text_processor = TextProcessor()

    print(f" Trying to validate input '42': {text_processor.validate(42)}")

    print(f"Processing data: {text_list}")
    for item in text_list:
        if text_processor.validate(item):
            text_processor.ingest(item)

    print("Extracting 1 value")
    for _ in range(1):
        try:
            index, value = text_processor.output()
            print(f" Text value {index}: {value}")
        except ValueError as e:
            print(f" Got exception: {e}")


def main_log() -> None:
    print("Testing LogProcessor...")

    log_list = [
        {"log_level": "NOTICE", "log_message": "Connection to server"},
        {"log_level": "ERROR", "log_message": "Unauthorized access"},
    ]
    log_processor = LogProcessor()

    print(f" Trying to validate input 'Hello': {log_processor.validate('Hello')}")

    print(f"Processing data: {log_list}")
    for item in log_list:
        if log_processor.validate(item):
            log_processor.ingest(item)

    print("Extracting 2 value")
    for _ in range(2):
        try:
            index, value = log_processor.output()
            print(f" Log entry {index}: {value}")
        except ValueError as e:
            print(f" Got exception: {e}")


if __name__ == "__main__":
    main_num()
    main_text()
    main_log()
