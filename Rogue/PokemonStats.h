#pragma once

// Libraries
#include <cstdint>

// Other
#include "Pokedex.hpp"

/*
* Struct containing all base stats for pokemon.
*/
struct PokemonStats
{
	/*
	* Constructs pokemon stats.
	*/
	explicit PokemonStats(const PODPokemon& pokemon);
	~PokemonStats() = default;

	PokemonStats(const PokemonStats& other) = default;
	PokemonStats(PokemonStats&& other) noexcept = default;
	PokemonStats& operator=(const PokemonStats& other) = default;
	PokemonStats& operator=(PokemonStats&& other) noexcept = default;

	/*
	* Increases the pokemon base stats by 1.
	*/
	PokemonStats& operator++();
	/*
	* Increases or decreases the pokemon base stats.
	* 
	* @param levelDifference: The difference between the new level and old level, can be negative.
	*/
	void operator+=(int levelDifference);

	int Health;
	int Attack;
	int SpecialAttack;
	int Defense;
	int SpecialDefense;
	int Speed;
};