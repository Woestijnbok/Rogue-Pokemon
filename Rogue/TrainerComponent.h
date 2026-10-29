#pragma once

// Core
#include "Component.h"
#include "Enums.h"
#include "Subject.h"

// Libraries
#include <memory>
#include <array>

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
	* Remove the pickup prompt.
	*/
	void ConfirmPickup();
	/*
	* Inceases the score of the trainer.
	* 
	* @param amount: The amount to increase the score by.
	*/
	void IncreaseScore(uint16_t amount);
	/*
	* Resets the trainer tot the starting.
	*/
	void Reset();
	/*
	* Tries to use the specified item, if the trainer still has the item in it's inventory.
	* 
	* @param item: The item we want to try use.
	* @return Wether or not we used the item.
	*/
	bool TryUseItem(const ItemType item);
	/*
	* Picks up the specified item and puts it in the player's inventory.
	* 
	* @param item: The item type to pickup
	*/
	void PickupItem(const ItemType item);
	/*
	* Retusn the on item use event
	* 
	* @return the on item use event
	*/
	Minigin::Subject<const ItemType>& OnItemUse();

private:
	/*
	* Creates the starter pokemon to use for battles
	* 
	* @return The starter pokemon.
	*/
	PokemonComponent* CreateStartPokemon() const;
	/*
	* Render's the current route / level starting from 1.
	*/
	void RenderRoute() const;
	/*
	* Heal the active pokemon back to full health.
	* Also disables the movement component and flags to show the pickup prompt.
	*/
	void HealActivePokemon();
	/*
	* Renders the item prompt for when picking up or using an item.
	*/
	void RenderItemPrompt() const;
	/*
	* Sets the show item prompt flag and disables or enables the movement component.
	* 
	* @param enable: Wether or not to enable or disable the movement component and how to set the flag.
	*/
	void SwitchItemPrompt(bool enable);

	// Wether or not we show the pickup prompt
	bool m_ShowItemPrompt;
	// Score based on which level the player is at
	uint16_t m_Score;
	// Active pokemon
	PokemonComponent* m_ActivePokemon;
	// Cached movement component
	MovementComponent* m_MovementComponent;
	// Cached infobox texture
	Minigin::Texture const * m_InfoBoxTexture;
	// Route texture
	const std::unique_ptr<Minigin::Texture> m_RouteTexture;
	// Item prompt text
	std::unique_ptr<Minigin::Text> m_ItemPromptText;
	// Inventory prompt text
	std::unique_ptr<Minigin::Text> m_InventoryPromptText;
	// Current Score / level / route text
	std::unique_ptr<Minigin::Text> m_RouteText;
	// Inventory of the player holding the amount left for each item.
	std::array<uint16_t, static_cast<size_t>(ItemType::Count)> m_Inventory;
	// The event for when an item is used
	Minigin::Subject<const ItemType> m_OnItemUse;
};