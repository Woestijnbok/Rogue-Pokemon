#pragma once

// Minigin
#include <Component.h>

// Standart Library
#include <array>

// Other
#include "Pokedex.hpp"

class PokedexComponent : public Minigin::Component
{
public:
	explicit PokedexComponent(Minigin::GameObject* owner);
	virtual ~PokedexComponent() = default;

	PokedexComponent(const PokedexComponent& other) = delete;
	PokedexComponent(PokedexComponent&& other) noexcept = delete;
	PokedexComponent& operator=(const PokedexComponent& other) = delete;
	PokedexComponent& operator=(PokedexComponent&& other) noexcept = delete;

	/*
	* Gets a certain element
	* 
	* @param index: The element index, starts from 1.
	*/
	const PODElement& GetElement(size_t index);
	/*
	* Gets a certain move
	*
	* @param index: The move index, starts from 1.
	*/
	const PODMove& GetMove(size_t index);
	/*
	* Gets a certain Pokemon
	*
	* @param index: The pokemon index, starts from 1.
	*/
	const PODPokemon& GetPokemon(size_t index);
	/*
	* Gets the type effectiveness multiplier
	* 
	* @param attacker: The attacker's move index starts from 1.
	* @param defender: The defender's pokemon index starts from 1.
	* @return the multiplier in float format i.e. 1.5f
	*/
	float GetTypeEffectiveness(size_t move, size_t defender);

private:
	std::array<PODElement, POKEDEX_ELEMENT_COUNT> m_Elements;
	std::array<PODMove, POKEDEX_MOVE_COUNT> m_Moves;
	std::array<PODPokemon, POKEDEX_POKEMON_COUNT> m_Pokemon;
	std::array<float, POKEDEX_ELEMENT_COUNT* POKEDEX_ELEMENT_COUNT> m_TypeMatchups;

	/*
	* Helper function to fill in the type matchups
	*
	* @return the type matchup array
	*/
	void MakeTypeMatchups();
};

