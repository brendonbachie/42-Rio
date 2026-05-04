from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):
    def __init__(self):
        self._storage = []
        self._counter = 0
        self.name = ""

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    @abstractmethod
    def ingest(self, data: Any) -> None:
        pass

    def total_processed(self) -> int:
        return self._counter

    def remaining(self) -> int:
        return len(self._storage)

    def processor_name(self) -> str:
        return self.name

    def output(self) -> tuple[int, str]:
        if not self._storage:
            raise ValueError("No data to output")
        return self._storage.pop(0)


class DataStream():
    def __init__(self):
        self._processors = []

    def register_processor(self, proc: DataProcessor) -> None:
        self._processors.append(proc)

    def process_stream(self, stream: list[Any]) -> None:
        for item in stream:
            processed = False
            for processor in self._processors:
                if processor.validate(item):
                    processor.ingest(item)
                    processed = True
                    break
            if not processed:
                print(f"DataStream error - "
                      f"Can't process element in stream: {item}")

    def print_processors_stats(self) -> None:
        print("== DataStream statistics ==")
        if not self._processors:
            print("No processor found, no data")
            return
        for processor in self._processors:
            print(f"{processor.processor_name()}: "
                  f"total {processor.total_processed()} items processed, "
                  f"remaining {processor.remaining()} on processor")


class NumericProcessor(DataProcessor):
    def __init__(self):
        super().__init__()
        self.name = "Numeric Processor"

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
        self.name = "Text Processor"

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
        self.name = "Log Processor"

    def validate(self, data: Any) -> bool:
        if isinstance(data, dict):
            if 'log_level' in data and 'log_message' in data:
                return True
        if isinstance(data, list):
            return all(isinstance(item, dict) and
                       'log_level' in item and 'log_message'
                       in item for item in data)
        return False

    def ingest(self, data: Any) -> None:
        if not self.validate(data):
            raise ValueError("Improper log data")
        if isinstance(data, dict):
            item = (self._counter,
                    f"{data['log_level']}: {data['log_message']}")
            self._storage.append(item)
            self._counter += 1
        elif isinstance(data, list):
            for item in data:
                item = (self._counter,
                        f"{item['log_level']}: {item['log_message']}")
                self._storage.append(item)
                self._counter += 1


def main() -> None:
    print("=== Code Nexus - Data Stream ===\n")

    num = NumericProcessor()
    text = TextProcessor()
    log = LogProcessor()

    print("Initialize Data Stream...")
    batch: list[Any] = ['Hello world',
                        [3.14, -1, 2.71],
                        [{'log_level': 'WARNING',
                          'log_message': 'Telnet access! Use ssh instead'},
                         {'log_level': 'INFO',
                          'log_message': 'User wil is connected'}],
                        42, ['Hi', 'five']]
    data_stream = DataStream()
    data_stream.print_processors_stats()
    print("\nRegistering Numeric processor\n")
    data_stream.register_processor(num)
    print(f"Send first batch of data on stream: {batch}")
    data_stream.process_stream(batch)
    data_stream.print_processors_stats()
    print("\nRegistering other data processors")
    data_stream.register_processor(text)
    data_stream.register_processor(log)
    print("Send the same batch again")
    data_stream.process_stream(batch)
    data_stream.print_processors_stats()
    print("\nConsume some elements"
          " from the data processors: Numeric 3, Text 2, Log 1")
    for _ in range(3):
        try:
            num.output()
        except ValueError as e:
            print(f"Error consuming from {num.processor_name()}: {e}")
    for _ in range(2):
        try:
            text.output()
        except ValueError as e:
            print(f"Error consuming from {text.processor_name()}: {e}")
    for _ in range(1):
        try:
            log.output()
        except ValueError as e:
            print(f"Error consuming from {log.processor_name()}: {e}")

    data_stream.print_processors_stats()


if __name__ == "__main__":
    main()
