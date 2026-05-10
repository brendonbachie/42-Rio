import ex0


def test_factory(factory: ex0.CreatureFactory):
    print("Testing factory")
    base = factory.create_base()
    evolved = factory.create_evolved()
    print(base.describe())
    print(base.attack())
    print(evolved.describe())
    print(evolved.attack())


def test_battle(factory1: ex0.CreatureFactory, factory2: ex0.CreatureFactory):
    print("Testing battle")
    creature1 = factory1.create_base()
    creature2 = factory2.create_base()
    print(creature1.describe())
    print(" vs.")
    print(creature2.describe())
    print(" fight!")
    print(creature1.attack())
    print(creature2.attack())


if __name__ == "__main__":
    flame_factory = ex0.FlameFactory()
    aqua_factory = ex0.AquaFactory()
    test_factory(flame_factory)
    print("...")
    test_factory(aqua_factory)
    print("...")
    test_battle(flame_factory, aqua_factory)
