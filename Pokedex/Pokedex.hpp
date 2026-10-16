#pragma once

#include <string>
#include <format>
#include <array>
#include <cstdint>
#include <iostream>

#define POKEDEX_MAX_NAME_LENGTH 50
#define POKEDEX_POKEMON_COUNT 202
#define POKEDEX_POKEMON_LEGENDARY_START 193
#define POKEDEX_ELEMENT_COUNT 18
#define POKEDEX_MOVE_COUNT 62

struct PODElement
{
	PODElement() noexcept = default;
	~PODElement() = default;

	PODElement(const PODElement& other) = default;
	PODElement(PODElement&& other) noexcept = default;
	PODElement& operator=(const PODElement& other) = default;
	PODElement& operator=(PODElement&& other) noexcept = default;

	char Name[POKEDEX_MAX_NAME_LENGTH];
	uint8_t ID;
	uint8_t SuperEffectiveAgainst[5];
	uint8_t NotVeryEffectiveAgainst[7];
	uint8_t IneffectiveAgainst;
};

struct PODMove final
{
	PODMove() noexcept = default;
	~PODMove() = default;

	PODMove(const PODMove& other) = default;
	PODMove(PODMove&& other) noexcept = default;
	PODMove& operator=(const PODMove& other) = default;
	PODMove& operator=(PODMove&& other) noexcept = default;

	char Name[POKEDEX_MAX_NAME_LENGTH];
	uint8_t ID;
	uint8_t MoveType; // 1 for special 0 for physical
	uint8_t ElementType;
	uint8_t Power;
	uint8_t Accuracy;
};

struct PODPokemon final
{
	PODPokemon() noexcept = default;
	~PODPokemon() = default;

	PODPokemon(const PODPokemon& other) = default;
	PODPokemon(PODPokemon&& other) noexcept = default;
	PODPokemon& operator=(const PODPokemon& other) = default;
	PODPokemon& operator=(PODPokemon&& other) noexcept = default;

	char Name[POKEDEX_MAX_NAME_LENGTH];
	uint8_t Types[2];
	uint8_t ID;
	uint8_t PhysicalAttack;
	uint8_t SpecicalAttack;
	uint8_t PhysicalDefense;
	uint8_t SpecialDefense;
	uint8_t Speed;
	uint8_t Health;
	uint8_t Moves[4];
};

inline std::ostream& operator<<(std::ostream& stream, const PODElement& element)
{
	return stream << std::format("Name: {}", element.Name);
}

inline std::ostream& operator<<(std::ostream& stream, const PODMove& move)
{
	return stream << std::format("Name: {}", move.Name);
}

inline std::ostream& operator<<(std::ostream& stream, const PODPokemon& pokemon)
{
	return stream << std::format("Name: {}", pokemon.Name);
}