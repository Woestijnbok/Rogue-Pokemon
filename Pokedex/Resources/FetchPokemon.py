"""Fills the six stat fields and the ElementTypes field in pokemon.json from PokeAPI.
Usage:  python fetch_stats.py pokemon.json elements.json   (needs internet; stdlib only)
Results are cached in PokemonCache.json so re-runs don't hit the API again."""
import json, os, sys, time, urllib.request

API = "https://pokeapi.co/api/v2"
CACHE = "PokemonCache.json"
# PokeAPI stat name -> your JSON key
STAT_MAP = {
    "hp": "Health",
    "attack": "PhysicalAttack",
    "defense": "PhysicalDefense",
    "special-attack": "SpecialAttack",
    "special-defense": "SpecialDefense",
    "speed": "Speed",
}

def get(url):
    req = urllib.request.Request(url, headers={"User-Agent": "rogue-pokemon-tool"})
    with urllib.request.urlopen(req, timeout=30) as r:
        return json.load(r)

def fetch_data(name):
    slug = name.lower().replace(" ", "-")
    # Look up the species first: it resolves to the default form
    # (e.g. "deoxys" -> "deoxys-normal"), which /pokemon/<name> may not accept.
    species = get(f"{API}/pokemon-species/{slug}")
    default = next(v for v in species["varieties"] if v["is_default"])
    poke = get(default["pokemon"]["url"])
    stats = {STAT_MAP[s["stat"]["name"]]: s["base_stat"]
             for s in poke["stats"] if s["stat"]["name"] in STAT_MAP}
    types = [t["type"]["name"] for t in sorted(poke["types"], key=lambda t: t["slot"])]
    return {"stats": stats, "types": types}

def main(path, elements_path):
    elem_id = {e["Name"].lower(): e["ID"] for e in json.load(open(elements_path))}
    cache = json.load(open(CACHE)) if os.path.exists(CACHE) else {}
    pokemon = json.load(open(path))
    for p in pokemon:
        name = p["Name"]
        # entries from the very first script version have no "types" key, so they are refetched
        if "types" not in cache.get(name, {}):
            try:
                cache[name] = fetch_data(name)
            except Exception as e:
                print(f"FAILED {name}: {e}")
                continue
            json.dump(cache, open(CACHE, "w"), indent=2)
            time.sleep(0.2)  # be polite to the free API
            print("fetched", name)
        entry = cache[name]
        p.update(entry["stats"])
        for k in STAT_MAP.values():
            assert 0 <= p[k] <= 255, (name, k)  # must fit in uint8_t
        ids = [elem_id[t] for t in entry["types"] if t in elem_id]
        if len(ids) != len(entry["types"]):
            print(f"note {name}: skipped types not in {elements_path}: "
                  f"{[t for t in entry['types'] if t not in elem_id]}")
        if not ids:
            print(f"WARNING {name}: no usable types, ElementTypes left as [0, 0]")
        p["ElementTypes"] = (ids + [0, 0])[:2]
    json.dump(pokemon, open(path, "w"), indent=2)

if __name__ == "__main__":
    main(*(sys.argv[1:3] or ["Pokemon.json", "Elements.json"]))