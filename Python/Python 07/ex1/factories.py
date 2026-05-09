from ex0.factories import CreatureFactory

class HealingCreatureFactory(CreatureFactory):

    def create_base(self):
        return HealingCreature()

    def create_evolved(self):
        return HealingCreatureEvolved()