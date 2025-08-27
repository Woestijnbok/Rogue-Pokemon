#pragma once

// Libraries
#include <cstdint>

class PokemonComponent;

/*
* Reads pokemon information from Pokedex.bin (binary pokedex file) to create pokemon component.
* 
* @param pokedexIndex: The pokedex index that will be used to read the correct pokemon in the pokedex file.
* 
* @return The pokemon component based on the given pokedex information.
*/
PokemonComponent* ReadPokemon(uint8_t pokedexIndex);