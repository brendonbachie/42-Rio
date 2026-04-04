from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):

    @abstractmethod
    def process(self, data: Any) -> str:
        pass

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    def format_output(self, result: str) -> str:
        return f"Output: {result}"


class NumericProcessor(DataProcessor):

    def process(self, data: Any) -> str:
        if not self.validate(data):
            raise ValueError("Invalid data for NumericProcessor")
        total = sum(data)
        avg = total / len(data)
        msg = f"Processed {len(data)} numeric values, sum={total}, avg={avg}"
        return super().format_output(msg)

    def validate(self, data: Any) -> bool:
        return (isinstance(data, list) and len(data) > 0 and
                all(isinstance(x, (int, float)) for x in data))


class TextProcessor(DataProcessor):

    def process(self, data: Any) -> str:
        if not self.validate(data):
            raise ValueError("Invalid data for TextProcessor")
        char_count = len(data)
        word_count = len(data.split())
        msg = (f"Processed text: {char_count} characters, "
               f"{word_count} words")
        return super().format_output(msg)

    def validate(self, data: Any) -> bool:
        return isinstance(data, str) and len(data) > 0


class LogProcessor(DataProcessor):

    def process(self, data: Any) -> str:
        if not self.validate(data):
            raise ValueError("Invalid data for LogProcessor")
        type_entry = data.split(':')[0]
        log = data.split(':')[1].strip()
        if type_entry == "INFO":
            type = "INFO"
        elif type_entry == "ERROR":
            type = "ALERT"
        msg = f"[{type}] {type_entry} level detected: {log}"
        return super().format_output(msg)

    def validate(self, data: Any) -> bool:
        return isinstance(data, str) and ':' in data


def main():
    print("=== CODE NEXUS - DATA PROCESSOR FOUNDATION ===\n")
    print("Initializing Numeric Processor...")
    numeric_processor = [1, 2, 3, 4, 5]
    text_processor = "Hello Nexus World"
    log_processor = "INFO: System initialized successfully"

    try:
        print(f"Processing data: {numeric_processor}")
        print("Validation: Numeric data verified")
        print(NumericProcessor().process(numeric_processor))
    except ValueError as e:
        print(e)
    print("\nInitializing Text Processor...")
    try:
        print(f"Processing data: {text_processor}")
        print("Validation: Text data verified")
        print(TextProcessor().process(text_processor))
    except ValueError as e:
        print(e)
    print("\nInitializing Log Processor...")
    try:
        print(f"Processing data: {log_processor}")
        print("Validation: Log entry verified")
        print(LogProcessor().process(log_processor))
    except ValueError as e:
        print(e)
    print("\n=== Polymorphic Processing Demo ===")
    print("Processing multiple data types through same interface...")
    try:
        print("Result 1:", NumericProcessor().process(numeric_processor))
        print("Result 2:", TextProcessor().process(text_processor))
        print("Result 3:", LogProcessor().process(log_processor))
    except ValueError as e:
        print(e)
    print("\nFoundation systems online. Nexus ready for advanced streams")


if __name__ == "__main__":
    main()
