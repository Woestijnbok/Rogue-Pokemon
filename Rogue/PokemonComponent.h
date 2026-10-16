#pragma once

// Libraries
#include <array>
#include <string>
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
	* Gets some the stats of the pokemon.
	* 
	* @return The stats of the pokemon.
	*/
	const PokemonStats& GetStats();
	/*
	* Gets the current pokemon health.
	*
	* @return The current health of the pokemon.
	*/
	uint16_t GetCurrentHealth();
	/*
	* Sets the current pokemon health.
	*
	* @param health: The new health of the pokemon.
	*/
	void SetCurrentHealth(uint16_t health);
	/*
	* Makes the pokemon take damage.
	*
	* @param damage: The damage to take.
	*/
	void TakeDamage(uint16_t damage);
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
	* Returns the texture of the pokemon.
	*
	* @return The texture of the pokemon
	*/
	Minigin::Texture const * GetTexture() const;
	/*
	* Returns the name of the pokemon.
	*
	* @return The name of the pokemon
	*/
	bool IsWild() const;
	/*
	* Gets the pokemon's health in percentage.
	*
	* @return The pokemon's health.
	*/
	float GetHealthPercentage() const;
	/*
	* Chechs wether or not the pokemon is dead.
	*
	* @return true if pokemon fainted.
	*/
	bool IsDead() const;
	/*
	* Getter for the trainer of this pokemon.
	* 
	* @return pointer to trainer component, nullptr if wild pokemon.
	*/
	TrainerComponent const * GetTrainer() const;
	/*
	* Sets the pokemon's level, also clamps the level between 1 and 100.
	* Finnaly updates the level text to reflect the new level.
	* 
	* @param level: The new pokemon's level.
	*/
	void SetLevel(uint16_t level);
	/*
	* Getter for the pokemon ID.
	* 
	* @return The ID to look up the pokemon in the pokedex.
	*/
	size_t GetPokedexID();
	
private:
	// ID used to search in the pokemon in the pokedex
	size_t m_ID;
	// Current level of the pokemon
	uint16_t m_Level;
	// Current health of the pokemon different from m_Stats.Health which is max health
	uint16_t m_CurrentHealth;
	// Type indices of the pokemon, zero means no type
	const std::array<uint8_t, 2> m_Types;
	// Various pokemon stats
	PokemonStats m_Stats;
	// The move indices of the pokemon
	const std::array<Move, 4> m_Moves;
	// Pokemon name in text
	const std::unique_ptr<Minigin::Text> m_Name;
	// Pokemon level in text
	const std::unique_ptr<Minigin::Text> m_LevelText;
	// Pokemon texture
	const std::unique_ptr<Minigin::Texture> m_Texture;
	// Optional trainer, empty if a wild pokemon
	TrainerComponent const * m_Trainer;
};