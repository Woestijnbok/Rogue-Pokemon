#pragma once

// Libraries
#include <cstdint>

/*
* Struct containing all stats for pokemon.
*/
struct PokemonStats
{
	/*
	* Constructs pokemon stats.
	*/
	explicit PokemonStats();
	~PokemonStats() = default;

	PokemonStats(const PokemonStats& other) = default;
	PokemonStats(PokemonStats&& other) noexcept = default;
	PokemonStats& operator=(const PokemonStats& other) = default;
	PokemonStats& operator=(PokemonStats&& other) noexcept = default;

	uint16_t Level;
	uint16_t MaxHealth;
	uint16_t CurrentHealth;
	uint16_t Attack;
	uint16_t SpecialAttack;
	uint16_t Defense;
	uint16_t SpecialDefense;
	uint16_t Speed;
};