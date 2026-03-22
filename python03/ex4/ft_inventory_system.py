import sys


def parse_inventory(args):
    inventory = {}
    for arg in args[1:]:
        try:
            item, quantity = arg.split(':')
        except ValueError:
            print(f"Error- invalid parameter '{arg}'")
            continue
        try:
            quantity = int(quantity)
            if item in inventory:
                print(f"Redundant item '{item}'- discarding")
            else:
                inventory[item] = quantity
        except ValueError as e:
            print(f"Quantity error for '{item}': {e}")
    print(f"Got inventory: {inventory}")
    return inventory


def main():
    print("=== Inventory System Analysis ===")

    inventory = parse_inventory(sys.argv)
    items = list(inventory.keys())
    values = list(inventory.values())
    total = sum(values)
    most_abundant = ['', 0]
    least_abundant = ['', 0]

    print(f"Item list: {items}")
    print(f"Total quantity of the {len(items)} items: {total}")

    for item in inventory:
        percentage = round((inventory[item] / total * 100), 1)
        print(f"Item {item} represents {percentage}%")

    for item in inventory:
        if inventory[item] > most_abundant[1]:
            most_abundant = [item, inventory[item]]
        if least_abundant[1] == 0 or inventory[item] < least_abundant[1]:
            least_abundant = [item, inventory[item]]

    print(f"Item most abundant: {most_abundant[0]} with quantity "
          f"{most_abundant[1]}")
    print(f"Item least abundant: {least_abundant[0]} with quantity "
          f"{least_abundant[1]}")

    inventory.update({'magic_item': 1})
    print(f"Updated inventory: {inventory}")


if __name__ == "__main__":
    main()
