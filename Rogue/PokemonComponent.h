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
#include "PokemonStats.h"

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
	explicit PokemonComponent(Minigin::GameObject* owner, const PODPokemon& pokemon, TrainerComponent const * trainer);
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
	* Gets all the stats of the pokemon.
	* 
	* @return The stats of the pokemon.
	*/
	PokemonStats& GetStats();
	/*
	* Returns all moves this pokemon has, aka 4.
	*
	* @return The moves array.
	*/
	const std::array<Move, 4>& GetMoves() const;
	/*
	* Returns the name of the pokemon in text.
	*
	* @return The name of the pokemon in text.
	*/
	Minigin::Text * GetNameText() const;
	/*
	* Returns the level of the pokemon in text.
	*
	* @return The level of the pokemon in text.
	*/
	Minigin::Text* GetLevelText() const;
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
	/*
	* Sets the level of the pokemon.
	* 
	* @param level: The new pokemon's level.
	*/
	void SetLevel(uint8_t level);
	/*
	* Gets the pokemon's health in percentage.
	*
	* @return The pokemon's health.
	*/
	float GetHealthPercentage() const;
	
private:
	// Pokemon level number
	PokemonStats m_Stats;
	// The moves / attacks of the pokemon
	const std::array<Move, 4> m_Moves;
	// Pokemon name
	const std::unique_ptr<Minigin::Text> m_Name;
	// Pokemon level in text
	const std::unique_ptr<Minigin::Text> m_LevelText;
	// Pokemon texture
	const std::unique_ptr<Minigin::Texture> m_Texture;
	// Optional trainer, empty if a wild pokemon
	const std::optional<TrainerComponent const *> m_Trainer;
};