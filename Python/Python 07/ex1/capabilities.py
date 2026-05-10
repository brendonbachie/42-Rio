from abc import ABC, abstractmethod


class HealCapability(ABC):
    def __init__(self) -> None:
        super().__init__()

    @abstractmethod
    def heal(self) -> str:
        pass


class TransformCapability(ABC):
    def __init__(self):
        super().__init__()
        self.transformed = False

    @abstractmethod
    def transform(self) -> str:
        pass

    @abstractmethod
    def revert(self) -> str:
        pass
