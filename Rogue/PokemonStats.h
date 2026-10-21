#pragma once

// Libraries
#include <cstdint>

// Other
#include "Pokedex.hpp"

/*
* Struct containing all stats for pokemon.
*/
struct PokemonStats
{
	/*
	* Constructs pokemon stats.
	*/
	explicit PokemonStats(const PODPokemon& pokemon);
	explicit PokemonStats(uint16_t health, uint16_t speed, uint16_t attack, uint16_t defense, uint16_t specialAttack, uint16_t specialDefense);
	~PokemonStats() = default;

	PokemonStats(const PokemonStats& other) = default;
	PokemonStats(PokemonStats&& other) noexcept = default;
	PokemonStats& operator=(const PokemonStats& other) = default;
	PokemonStats& operator=(PokemonStats&& other) noexcept = default;

	PokemonStats& operator++();
	PokemonStats operator+(uint16_t levels) const;

	uint16_t Health;
	uint16_t Attack;
	uint16_t SpecialAttack;
	uint16_t Defense;
	uint16_t SpecialDefense;
	uint16_t Speed;
};