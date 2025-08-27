#pragma once

#include "Component.h"

class PokemonComponent;

/*
* Will handle all logic handling trainer's pokemon and trainer's progression.
*/
class TrainerComponent final : public Minigin::Component
{
public:
	/*
	* Constructs trainer component.
	* 
	* @owner The game object that will own this component.
	*/
	explicit TrainerComponent(Minigin::GameObject* owner);
	virtual ~TrainerComponent() = default;

	TrainerComponent(const TrainerComponent& other) = delete;
	TrainerComponent(TrainerComponent&& other) noexcept = delete;
	TrainerComponent& operator=(const TrainerComponent& other) = delete;
	TrainerComponent& operator=(TrainerComponent&& other) noexcept = delete;

	/*
	* Get the active pokemon, aka the pokemon to fight encounters with.
	* 
	* @return The active pokemon.
	*/
	PokemonComponent* GetActivePokemon() const;

private:
	// Active pokemon
	PokemonComponent* m_ActivePokemon;
};