# > python3 capacitor.py
# Testing Creature with healing capability
# base:
# Sproutling is a Grass type Creature
# Sproutling uses Vine Whip!
# Sproutling heals itself for a small amount
# evolved:
# Bloomelle is a Grass/Fairy type Creature
# Bloomelle uses Petal Dance!
# Bloomelle heals itself and others for a large amount
# Testing Creature with transform capability
# base:
# Shiftling is a Normal type Creature
# Shiftling attacks normally.
# Shiftling shifts into a sharper form!
# Shiftling performs a boosted strike!
# Shiftling returns to normal.
# evolved:
# Morphagon is a Normal/Dragon type Creature
# Morphagon attacks normally.
# Morphagon morphs into a dragonic battle form!
# Morphagon unleashes a devastating morph strike!
# Morphagon stabilizes its form.

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