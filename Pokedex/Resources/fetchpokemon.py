import requests
import json
import os
import time

# Hardcoded Hoenn/Emerald Dex (1–202)
hoenn_pokedex = [
    "Treecko", "Grovyle", "Sceptile", "Torchic", "Combusken", "Blaziken",
    "Mudkip", "Marshtomp", "Swampert", "Poochyena", "Mightyena", "Zigzagoon",
    "Linoone", "Wurmple", "Silcoon", "Beautifly", "Cascoon", "Dustox",
    "Lotad", "Lombre", "Ludicolo", "Seedot", "Nuzleaf", "Shiftry",
    "Taillow", "Swellow", "Wingull", "Pelipper", "Ralts", "Kirlia", "Gardevoir",
    "Surskit", "Masquerain", "Shroomish", "Breloom", "Slakoth", "Vigoroth", "Slaking",
    "Abra", "Kadabra", "Alakazam", "Nincada", "Ninjask", "Shedinja",
    "Whismur", "Loudred", "Exploud", "Makuhita", "Hariyama", "Goldeen", "Seaking",
    "Magikarp", "Gyarados", "Azurill", "Marill", "Azumarill", "Geodude",
    "Graveler", "Golem", "Nosepass", "Skitty", "Delcatty", "Zubat", "Golbat",
    "Crobat", "Tentacool", "Tentacruel", "Sableye", "Mawile", "Aron", "Lairon",
    "Aggron", "Machop", "Machoke", "Machamp", "Meditite", "Medicham", "Electrike",
    "Manectric", "Plusle", "Minun", "Magnemite", "Magneton", "Voltorb", "Electrode",
    "Volbeat", "Illumise", "Oddish", "Gloom", "Vileplume", "Bellossom", "Doduo",
    "Dodrio", "Roselia", "Gulpin", "Swalot", "Carvanha", "Sharpedo", "Wailmer",
    "Wailord", "Numel", "Camerupt", "Slugma", "Magcargo", "Torkoal", "Grimer",
    "Muk", "Koffing", "Weezing", "Spoink", "Grumpig", "Sandshrew", "Sandslash",
    "Spinda", "Skarmory", "Trapinch", "Vibrava", "Flygon", "Cacnea", "Cacturne",
    "Swablu", "Altaria", "Zangoose", "Seviper", "Lunatone", "Solrock", "Barboach",
    "Whiscash", "Corphish", "Crawdaunt", "Baltoy", "Claydol", "Lileep", "Cradily",
    "Anorith", "Armaldo", "Igglybuff", "Jigglypuff", "Wigglytuff", "Feebas",
    "Milotic", "Castform", "Staryu", "Starmie", "Kecleon", "Shuppet", "Banette",
    "Duskull", "Dusclops", "Tropius", "Chimecho", "Absol", "Vulpix", "Ninetales",
    "Pichu", "Pikachu", "Raichu", "Psyduck", "Golduck", "Wynaut", "Wobbuffet",
    "Natu", "Xatu", "Girafarig", "Phanpy", "Donphan", "Pinsir", "Heracross",
    "Rhyhorn", "Rhydon", "Snorunt", "Glalie", "Spheal", "Sealeo", "Walrein",
    "Clamperl", "Huntail", "Gorebyss", "Relicanth", "Corsola", "Chinchou",
    "Lanturn", "Luvdisc", "Horsea", "Seadra", "Kingdra", "Bagon", "Shelgon",
    "Salamence", "Beldum", "Metang", "Metagross", "Regirock", "Regice", "Registeel",
    "Latias", "Latios", "Kyogre", "Groudon", "Rayquaza", "Jirachi", "Deoxys"
]


# Folder to save sprites
SPRITE_FOLDER = "Pokemon"
os.makedirs(SPRITE_FOLDER, exist_ok=True)

# Fetch first 4 moves of a Pokémon
def get_pokemon_moves(pokemon_name):
    try:
        url = f"https://pokeapi.co/api/v2/pokemon/{pokemon_name}"
        response = requests.get(url)
        response.raise_for_status()
        moves_data = response.json().get("moves", [])
        
        move_list = []
        for move_entry in moves_data[:4]:  # First 4 moves
            try:
                move_url = move_entry["move"]["url"]
                move_resp = requests.get(move_url)
                move_resp.raise_for_status()
                move_info = move_resp.json()
                move_list.append({
                    "name": move_info.get("name", "unknown"),
                    "power": move_info.get("power") or 0
                })
                time.sleep(0.1)
            except Exception:
                move_list.append({"name": "unknown", "power": 0})
        return move_list
    except Exception:
        return [{"name": "unknown", "power": 0} for _ in range(4)]

# Download sprite if not exists
def download_sprite(pokemon_name):
    filename = f"{SPRITE_FOLDER}/{pokemon_name.capitalize()}.png"
    if os.path.exists(filename):
        print(f"Sprite already exists for {pokemon_name.capitalize()}, skipping download.")
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

# Compile Pokémon data
def compile_pokemon_data():
    pokemon_data = []
    for index, name in enumerate(hoenn_pokedex, start=1):
        moves = get_pokemon_moves(name)
        pokemon_data.append({
            "index": index,
            "name": name.capitalize(),
            "moves": moves
        })
        print(f"Fetched {name.capitalize()} (Emerald Dex #{index})")
        download_sprite(name)
    return pokemon_data

# Save JSON
def save_to_json(data, filename="Pokemon.json"):
    # Dump normally with indentation
    json_str = json.dumps(data, indent=2)
    
    # Replace "moves": [ with "moves":\n[
    json_str = json_str.replace('"moves": [', '"moves":\n    [')
    
    # Write to file
    with open(filename, "w", encoding="utf-8") as f:
        f.write(json_str)
    
    print(f"Data saved to {filename}")

# Main
if __name__ == "__main__":
    data = compile_pokemon_data()
    save_to_json(data)
