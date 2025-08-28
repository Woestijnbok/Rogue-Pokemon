import requests
import os
import time

# Hardcoded Hoenn/Emerald Dex (1–202)
hoenn_pokedex = [
    "Treecko", "Grovyle", "Sceptile", "Torchic", "Combusken", "Blaziken",
    "Mudkip", "Marshtomp", "Swampert", "Poochyena", "Mightyena", "Zigzagoon",
    "Linoone", "Wurmple", "Silcoon", "Beautifly", "Cascoon", "Dustox",
    # ... rest unchanged ...
    "Latias", "Latios", "Kyogre", "Groudon", "Rayquaza", "Jirachi", "Deoxys"
]

# Folder to save sprites
SPRITE_FOLDER = "Textures\Pokemon"
os.makedirs(SPRITE_FOLDER, exist_ok=True)

# Download sprite if not exists
def download_sprite(pokemon_name):
    filename = f"{SPRITE_FOLDER}/{pokemon_name.capitalize()}.png"
    if os.path.exists(filename):
        print(f"Sprite already exists for {pokemon_name.capitalize()}, skipping.")
        return
    try:
        url = f"https://pokeapi.co/api/v2/pokemon/{pokemon_name}"
        response = requests.get(url)
        response.raise_for_status()
        data = response.json()
        
        sprite_url = data.get("sprites", {}).get("front_default")
        if sprite_url:
            response = requests.get(sprite_url)
            response.raise_for_status()
            with open(filename, "wb") as f:
                f.write(response.content)
            print(f"Downloaded sprite for {pokemon_name.capitalize()}")
            time.sleep(0.1)
        else:
            print(f"No sprite found for {pokemon_name.capitalize()}")
    except Exception:
        print(f"Failed to download sprite for {pokemon_name.capitalize()}")

# Main
if __name__ == "__main__":
    for name in hoenn_pokedex:
        download_sprite(name)
