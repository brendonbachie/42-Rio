from ex0.creature import Creature
from .capabilities import HealCapability, TransformCapability


class Sproutling(Creature, HealCapability):
    def __init__(self) -> None:
        super().__init__("Sproutling", "Grass")

    def attack(self) -> str:
        return f"{self.name} uses Vine Whip!"

    def heal(self) -> str:
        return f"{self.name} heals itself for a small amount"


class Bloomelle(Creature, HealCapability):
    def __init__(self) -> None:
        super().__init__("Bloomelle", "Grass/Fairy")

    def attack(self) -> str:
        return f"{self.name} uses Petal Dance!"

    def heal(self) -> str:
        return f"{self.name} heals itself and others for a large amount"


class Shiftling(Creature, TransformCapability):
    def __init__(self) -> None:
        super().__init__("Shiftling", "Normal")

    def transform(self) -> str:
        if not self.transformed:
            self.transformed = True
            return f"{self.name} shifts into a sharper form!"
        else:
            return f"{self.name} is already transformed!"

    def attack(self) -> str:
        if not self.transformed:
            return f"{self.name} attacks normally."
        else:
            return f"{self.name} performs a boosted strike!"

    def revert(self) -> str:
        if self.transformed:
            self.transformed = False
            return f"{self.name} returns to normal."
        else:
            return f"{self.name} is already in its original form!"


class Morphagon(Creature, TransformCapability):
    def __init__(self) -> None:
        super().__init__("Morphagon", "Normal/Dragon")

    def transform(self) -> str:
        if not self.transformed:
            self.transformed = True
            return f"{self.name} morphs into a dragonic battle form!"
        else:
            return f"{self.name} is already transformed!"

    def attack(self) -> str:
        if not self.transformed:
            return f"{self.name} attacks normally."
        else:
            return f"{self.name} unleashes a devastating morph strike!"

    def revert(self) -> str:
        if self.transformed:
            self.transformed = False
            return f"{self.name} stabilizes its form."
        else:
            return f"{self.name} is already in its original form!"
