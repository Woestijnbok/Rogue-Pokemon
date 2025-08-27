#pragma once

// Libraries
#include <array>
#include <string>

// Core
#include "Component.h"

// Other
#include "Move.h"

struct PODPokemon;

/*
* Handles all information and life time of a pokemon.
* The actual pokemon will be the owning game object
*/
class PokemonComponent final : public Minigin::Component
{
public:
	/*
	* Construct pokemon component.
	* 
	* @param owner: The owning game object that will be the pokemon object.
	* @param pokemon: The plain old data struct for a pokemon containing all information.
	*/
	explicit PokemonComponent(Minigin::GameObject* owner, const PODPokemon& pokemon);
	virtual ~PokemonComponent() = default;

	PokemonComponent(const PokemonComponent& other) = delete;
	PokemonComponent(PokemonComponent&& other) noexcept = delete;
	PokemonComponent& operator=(const PokemonComponent& other) = delete;
	PokemonComponent& operator=(PokemonComponent&& other) noexcept = delete;

	/*
	* Returns the name of the pokemon.
	* 
	* @return The name.
	*/
	const std::string& GetName() const;

private:
	// The moves / attacks of the pokemon
	const std::array<Move, 4> m_Moves;
	// Pokemon name
	const std::string m_Name;
};