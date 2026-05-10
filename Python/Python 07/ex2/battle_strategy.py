from .strategy import BattleStrategy
from ex0.creature import Creature
from ex1.capabilities import HealCapability, TransformCapability
from typing import cast


class NormalStrategy(BattleStrategy):
    def is_valid(self, creature: Creature) -> bool:
        return isinstance(creature, Creature)

    def act(self, creature: Creature) -> str:
        if not self.is_valid(creature):
            raise ValueError(f"Invalid Creature "
                             f"'{creature.name}' for this normal strategy")
        return creature.attack()


class DefensiveStrategy(BattleStrategy):
    def is_valid(self, creature: Creature) -> bool:
        return isinstance(creature, HealCapability)

    def act(self, creature: Creature) -> str:
        if not self.is_valid(creature):
            raise ValueError(f"Invalid Creature "
                             f"'{creature.name}' for this defensive strategy")
        healer = cast(HealCapability, creature)
        result = creature.attack() + "\n"
        result += healer.heal()
        return result


class AggressiveStrategy(BattleStrategy):
    def is_valid(self, creature: Creature) -> bool:
        return isinstance(creature, TransformCapability)

    def act(self, creature: Creature) -> str:
        if not self.is_valid(creature):
            raise ValueError(f"Invalid Creature "
                             f"'{creature.name}' for this aggressive strategy")
        transformer = cast(TransformCapability, creature)
        result = creature.attack() + "\n"
        result += transformer.transform() + "\n"
        result += creature.attack() + "\n"
        result += transformer.revert()
        return result
