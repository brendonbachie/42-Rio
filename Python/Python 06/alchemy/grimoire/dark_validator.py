from .dark_spellbook import dark_spell_allowed_ingredients  # type: ignore


def validate_ingredients(ingredients: str) -> str:
    allowed = dark_spell_allowed_ingredients()
    ingredients_lower = ingredients.lower()

    for item in allowed:
        if item in ingredients_lower:
            return f"{ingredients} -> VALID"

    return f"{ingredients} -> INVALID"
