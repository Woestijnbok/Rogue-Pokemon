import requests
import json
import time

# Hardcoded Hoenn/Emerald Dex (1–202)
hoenn_pokedex = [
    "Treecko", "Grovyle", "Sceptile", "Torchic", "Combusken", "Blaziken",
    "Mudkip", "Marshtomp", "Swampert", "Poochyena", "Mightyena", "Zigzagoon",
    "Linoone", "Wurmple", "Silcoon", "Beautifly", "Cascoon", "Dustox",
    # ... rest unchanged ...
    "Latias", "Latios", "Kyogre", "Groudon", "Rayquaza", "Jirachi", "Deoxys"
]

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
    return pokemon_data

# Save JSON
def save_to_json(data, filename="Pokemon.json"):
    json_str = json.dumps(data, indent=2)
    json_str = json_str.replace('"moves": [', '"moves":\n    [')
    with open(filename, "w", encoding="utf-8") as f:
        f.write(json_str)
    print(f"Data saved to {filename}")

# Main
if __name__ == "__main__":
    data = compile_pokemon_data()
    save_to_json(data)
