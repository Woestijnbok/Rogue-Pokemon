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
	const PokemonStats& GetStats() const;
	/*
	* Gets the current pokemon health.
	*
	* @return The current health of the pokemon.
	*/
	int GetCurrentHealth() const;
	/*
	* Sets the current pokemon health.
	*
	* @param health: The new health of the pokemon.
	*/
	void SetCurrentHealth(int health);
	/*
	* Makes the pokemon take damage.
	*
	* @param damage: The damage to take.
	*/
	void TakeDamage(int damage);
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
	* Sets the pokemon's level adjusting it's stats, also clamps the level between 1 and 100.
	* Finnaly updates the level text to reflect the new level.
	* 
	* @param level: The new pokemon's level.
	*/
	void SetLevel(int level);
	/*
	* Levels the pokemon up by one increasing it's stats.
	* Also updating the level text to reflect the new level.
	*/
	void LevelUp();
	/*
	* Getter for the pokemon ID.
	* 
	* @return The ID to look up the pokemon in the pokedex.
	*/
	size_t GetPokedexID() const;
	/*
	* Checks Wether the pokemon is of a specific type.
	* 
	* @param type: The type index starting from 1.
	* @return True if it's the type.
	*/
	bool IsOfElement(size_t type) const;
	/*
	* Gets the elements / types of this pokemon.
	* 
	* @return The elements of this pokemon, second could be 0 if the pokemon only has 1 type / element
	*/
	std::pair<uint8_t, uint8_t> GetElements() const;
	/*
	* Gets the current level of the pokemon.
	* 
	* @return The level of the pokemon.
	*/
	int GetLevel() const;
	/*
	* Gains experience towards the next level and possibily levels up.
	* 
	* @param experience: The gained experience
	*/
	void GainExperience(int experience);
	/*
	* Resets the pokemon as if it where to start from level 1.
	*/
	void Reset();

private:
	// ID used to search in the pokemon in the pokedex
	size_t m_ID;
	// Current level of the pokemon
	int m_Level;
	// The current experience towards the next level.
	int m_Experience;
	// Various pokemon stats
	PokemonStats m_Stats;
	// Current health of the pokemon different from m_Stats.Health which is max health
	int m_CurrentHealth;
	// Type indices of the pokemon, zero means no type
	const std::array<uint8_t, 2> m_Types;
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