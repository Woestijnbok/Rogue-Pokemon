#pragma once

// Libraries
#include <cstdint>
#include <memory>
#include <random>

// Core
#include "Component.h"
#include "Subject.h"

namespace Minigin
{
	class Texture;
}

class PokemonComponent;
class TrainerComponent;

/*
* Manages battles between trainers and wild pokemons.
*/
class BattleManagerComponent final : public Minigin::Component
{
public:
	/*
	* Constructs the pokemon manager component.
	* 
	* @param owner: The game object that owns this component.
	*/
	explicit BattleManagerComponent(Minigin::GameObject* owner);
	virtual ~BattleManagerComponent() = default;

	BattleManagerComponent(const BattleManagerComponent& other) = delete;
	BattleManagerComponent(BattleManagerComponent&& other) noexcept = delete;
	BattleManagerComponent& operator=(const BattleManagerComponent& other) = delete;
	BattleManagerComponent& operator=(BattleManagerComponent&& other) noexcept = delete;

	/*
	* Renders the current battle.
	*/
	virtual void Render() const override;

	/*
	* Makes a battle between the trainer's active pokemon and a random wild pokemon.
	* The random wild pokemon has a chance to be legendary.
	* 
	* @param trainer: The trainer that will battle.
	*/
	void MakeBattle(TrainerComponent* trainer);
	/*
	* Ends the current battle deleting the wild pokemon and unloading the necessary resources.
	*/
	void EndBattle();
	/*
	* Checks wether or not there is an ongoing battle.
	* 
	* @return If there is an ongoing battle = true.
	*/
	bool InBattle() const;
	/*
	* Getter for the on battle started event.
	* 
	* @return The battle started event.
	*/
	Minigin::Subject<>& OnBattleStarted();
	/*
	* Getter for the on battle finished event.
	*
	* @return The battle finished event.
	*/
	Minigin::Subject<>& OnBattleFinished();

private:
	/*
	* Starts a battle between the two given pokemons.
	* 
	* @param trainer: The trainer's pokemon.
	* @param enemy: The wild pokemon.
	*/
	void StartBattle(PokemonComponent* trainer, PokemonComponent* enemy);

	// Current battle pair, first one being the trainer's pokemon and latter the wild pokemon
	std::pair<PokemonComponent*, PokemonComponent*> m_CurrentBattle;
	// Battle started event
	Minigin::Subject<> m_OnBattleStarted;
	// Battle finished event
	Minigin::Subject<> m_OnBattleFinished;
	// Battle background texture
	std::unique_ptr<Minigin::Texture> m_BattleBackground;
	// Trainer's active pokemon texture
	std::unique_ptr<Minigin::Texture> m_TrainerPokemonTexture;
	// Wild pokemon texture
	std::unique_ptr<Minigin::Texture> m_EnemyPokemonTexture;
	// Random device used for creating seeds
	std::random_device m_RandomDevice;
	// Random engine used to generate random values based on a seed.
	std::mt19937 m_RandomEngine;
	// Distribution used to determine chance going to a max of 100%
	std::uniform_real_distribution<float> m_ChanceDistribution;
	// Distribution spanning all common pokemon pokedex indices
	std::uniform_int_distribution<int> m_CommonDistribution;
	// Distribution spanning all legendary pokemon pokedex indices
	std::uniform_int_distribution<int> m_LegendaryDistribution;
	// Chance of encountering a legendary pokemon
	const float m_LegendaryChance;
};	