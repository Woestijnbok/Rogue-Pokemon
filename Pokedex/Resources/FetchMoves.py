"""Verifies/fills moves.json from PokeAPI (https://pokeapi.co).
Usage:  python fetch_moves.py moves.json elements.json   (stdlib only, needs internet)
Updates ElementType, MoveType and Accuracy. Power is NOT changed (it comes from your
original file); differences are only printed. Results are cached in moves_cache.json."""
import json, os, sys, time, urllib.request

API = "https://pokeapi.co/api/v2"
CACHE = "MovesCache.json"

def get(url):
    req = urllib.request.Request(url, headers={"User-Agent": "rogue-pokemon-tool"})
    with urllib.request.urlopen(req, timeout=30) as r:
        return json.load(r)

def main(moves_path, elements_path):
    elem_id = {e["Name"].lower(): e["ID"] for e in json.load(open(elements_path))}
    cache = json.load(open(CACHE)) if os.path.exists(CACHE) else {}
    moves = json.load(open(moves_path))
    for m in moves:
        slug = m["Name"].lower().replace(" ", "-")
        if slug not in cache:
            try:
                cache[slug] = get(f"{API}/move/{slug}")
            except Exception as e:
                print(f"FAILED {m['Name']}: {e}")
                continue
            # keep only what we need so the cache stays small
            d = cache[slug]
            cache[slug] = {"type": d["type"]["name"], "class": d["damage_class"]["name"],
                           "accuracy": d["accuracy"], "power": d["power"]}
            json.dump(cache, open(CACHE, "w"), indent=2)
            time.sleep(0.2)
            print("fetched", m["Name"])
        d = cache[slug]
        old = (m["ElementType"], m["MoveType"], m["Accuracy"])
        m["ElementType"] = elem_id[d["type"]]
        m["MoveType"] = 1 if d["class"] == "special" else 0   # status counts as 0
        if d["accuracy"] is not None:                          # null = never misses / self-target
            m["Accuracy"] = d["accuracy"]
        new = (m["ElementType"], m["MoveType"], m["Accuracy"])
        if old != new:
            print(f"changed {m['Name']}: (element, type, accuracy) {old} -> {new}")
        if d["power"] is not None and d["power"] != m["Power"]:
            print(f"note {m['Name']}: your Power={m['Power']}, PokeAPI Power={d['power']} (kept yours)")
    json.dump(moves, open(moves_path, "w"), indent=2)

if __name__ == "__main__":
    main(*(sys.argv[1:3] or ["Moves.json", "Elements.json"]))