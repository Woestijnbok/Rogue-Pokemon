#pragma once

// Core
#include "Component.h"

// Libraries
#include <memory>

class PokemonComponent;
class MovementComponent;

namespace Minigin
{
	class Texture;
	class Text;
}

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
	* Renders the world information regarding the player trainer.
	*/
	virtual void Render() const override;
	/*
	* Get the active pokemon, aka the pokemon to fight encounters with.
	* 
	* @return The active pokemon.
	*/
	PokemonComponent* GetActivePokemon() const;

	/*
	* Wether or not the trainer can pick up an item.
	*
	* @return true if the trainer can pick up an item.
	*/
	bool CanPickupItem() const;

	/*
	* Heal the active pokemon back to full health.
	* Also disables the movement component and flags to show the pickup prompt.
	*/
	void HealActivePokemon();

	/*
	* Remove the pickup prompt.
	*/
	void ConfirmPickup();

private:
	PokemonComponent* CreateStartPokemon() const;

	// Active pokemon
	PokemonComponent* m_ActivePokemon;
	// Wether or not we show the pickup prompt
	bool m_ShowPickupPrompt;
	// Cached movement component
	MovementComponent* m_MovementComponent;
	// Cached infobox texture
	Minigin::Texture const * m_InfoBoxTexture;
	// Cached pickup prompt text
	std::unique_ptr<Minigin::Text> m_PickupText;
};