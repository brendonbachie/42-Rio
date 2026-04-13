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


def main() -> None:
    num = NumericProcessor()
    if num.validate(42):
        num.ingest(42)
    if num.validate([1, 2, 3]):
        num.ingest([1, 2, 3])
    print(num.output())
    print(num.output())

if __name__ == "__main__":
    main()
