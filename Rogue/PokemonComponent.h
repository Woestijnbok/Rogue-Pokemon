#pragma once

// Libraries
#include <array>
#include <string>
#include <optional>
#include <memory>

// Core
#include "Component.h"

// Other
#include "Move.h"

namespace Minigin
{
	class Texture;
	class Text;
}

struct PODPokemon;
class TrainerComponent;

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
	* @param trainer: The trainer of this pokemon, use other constructor if wild pokemon.
	*/
	explicit PokemonComponent(Minigin::GameObject* owner, const PODPokemon& pokemon, TrainerComponent* trainer);
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
	* @return The name of the pokemon
	*/
	const std::string& GetName() const;
	/*
	* Returns the name of the pokemon.
	*
	* @return The name of the pokemon
	*/
	Minigin::Text const * GetText() const;
	/*
	* Returns the name of the pokemon.
	*
	* @return The name of the pokemon
	*/
	Minigin::Texture const * GetTexture() const;
	/*
	* Returns the name of the pokemon.
	*
	* @return The name of the pokemon
	*/
	bool IsWild() const;

private:
	// Pokemon name
	const std::unique_ptr<Minigin::Text> m_Name;
	// Pokemon texture
	const std::unique_ptr<Minigin::Texture> m_Texture;
	// The moves / attacks of the pokemon
	const std::array<Move, 4> m_Moves;
	// Optional trainer, empty if a wild pokemon
	const std::optional<TrainerComponent*> m_Trainer;
};