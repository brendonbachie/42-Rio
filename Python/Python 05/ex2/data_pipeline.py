from abc import ABC, abstractmethod
from typing import Any, Protocol


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

    def output_pipeline(self, nb: int, plugin: "ExportPlugin") -> None:
        for processor in self._processors:
            output_data = []
            for _ in range(nb):
                try:
                    output_data.append(processor.output())
                except ValueError:
                    break
            if output_data:
                plugin.process_output(output_data)


class ExportPlugin(Protocol):
    def process_output(self, data: list[tuple[int, str]]) -> None:
        ...


class CSVExportPlugin():
    def process_output(self, data: list[tuple[int, str]]) -> None:
        csv: str = ""
        csv = ",".join(item[1] for item in data)
        print("CSV Output:")
        print(csv)


class JSONExportPlugin():
    def process_output(self, data: list[tuple[int, str]]) -> None:
        pairs = ", ".join(f'"item_{item[0]}": "{item[1]}"' for item in data)
        print("JSON Output:")
        print("{" + pairs + "}")


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
                       'log_level' in item and
                       'log_message' in item for item in data)
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
    print("=== Code Nexus - Data Pipeline ===\n")

    num = NumericProcessor()
    text = TextProcessor()
    log = LogProcessor()

    print("Initialize Data Pipeline...")
    batch: list[Any] = ['Hello world',
                        [3.14, -1, 2.71],
                        [{'log_level': 'WARNING',
                          'log_message': 'Telnet access! Use ssh instead'},
                         {'log_level': 'INFO',
                            'log_message': 'User wil is connected'}],
                        42, ['Hi', 'five']]
    batch2: list[Any] = [21,
                         ['I love AI', 'LLMs are wonderful', 'Stay healthy'],
                         [{'log_level': 'ERROR',
                           'log_message': '500 server crash'},
                          {'log_level': 'NOTICE',
                           'log_message': 'Certificate expires in 10 days'}],
                         [32, 42, 64, 84, 128, 168], 'World hello'
                         ]
    data_stream = DataStream()
    data_stream.print_processors_stats()
    print("\nRegistering processors\n")
    data_stream.register_processor(num)
    data_stream.register_processor(text)
    data_stream.register_processor(log)
    print(f"Send first batch of data on stream: {batch}")
    data_stream.process_stream(batch)
    data_stream.print_processors_stats()
    print("\nSend 3 processed data from each processor to a CSV plugin")
    csv_plugin = CSVExportPlugin()
    data_stream.output_pipeline(3, csv_plugin)
    print("Send another batch of data")
    data_stream.process_stream(batch2)
    data_stream.print_processors_stats()
    print("\nSend 5 processed data from each processor to a JSON plugin")
    json_plugin = JSONExportPlugin()
    data_stream.output_pipeline(5, json_plugin)
    data_stream.print_processors_stats()


if __name__ == "__main__":
    main()
