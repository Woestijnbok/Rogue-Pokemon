#pragma once

#include <string>
#include <format>
#include <array>
#include <cstdint>
#include <iostream>

#define MAX_NAME_LENGTH 50
#define POKEDEX_COUNT 202
#define POKEDEX_LEGENDARY_START 193

struct PODMove final
{
	PODMove() noexcept = default;
	~PODMove() = default;

	PODMove(const PODMove& other) = default;
	PODMove(PODMove&& other) noexcept = default;
	PODMove& operator=(const PODMove& other) = default;
	PODMove& operator=(PODMove&& other) noexcept = default;

	char Name[MAX_NAME_LENGTH];
	uint8_t Power;
};

struct PODPokemon final
{
	PODPokemon() noexcept = default;
	~PODPokemon() = default;

	PODPokemon(const PODPokemon& other) = default;
	PODPokemon(PODPokemon&& other) noexcept = default;
	PODPokemon& operator=(const PODPokemon& other) = default;
	PODPokemon& operator=(PODPokemon&& other) noexcept = default;

	uint8_t Index;
	char Name[MAX_NAME_LENGTH];
	std::array<PODMove, 4> Moves;
};

inline std::ostream& operator<<(std::ostream& stream, const PODPokemon& pokemon)
{
	return stream << std::format
	("Name: {}, Index: {}, Moves: {}({}) & {}({}) & {}({}) & {}({})", pokemon.Name, pokemon.Index, pokemon.Moves[0].Name, pokemon.Moves[0].Power
	, pokemon.Moves[1].Name, pokemon.Moves[1].Power, pokemon.Moves[2].Name, pokemon.Moves[2].Power, pokemon.Moves[3].Name, pokemon.Moves[3].Power
	);
}