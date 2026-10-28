#include "TrainerComponent.h"

// Libraries
#include <format>

// Core
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Engine.h"

// Components
#include "PokemonComponent.h"
#include "MovementComponent.h"
#include "PokedexComponent.h"

// Core
#include "ResourceManager.h"
#include "Texture.h"
#include "Text.h"
#include "Renderer.h"

// Other
#include "Helpers.h"
#include "Pokedex.hpp"
#include "BattleManagerComponent.h"

using namespace Minigin;

TrainerComponent::TrainerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_ShowItemPrompt{ false },
	m_Score{ 1 },
	m_ActivePokemon{ CreateStartPokemon() },
	m_MovementComponent{ owner->GetComponent<MovementComponent>() },
	m_InfoBoxTexture{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<BattleManagerComponent>()->GetInfoBoxTexture() },
	m_RouteTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Route Sign.png") },
	m_ItemPromptText{ new Text{ "...", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_InventoryPromptText{ new Text{ "...", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_RouteText{ new Text{ "Route 1", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 25), Color::Black } },
	m_Inventory{}
{
	SetRenderOrder(RenderOrder::UI);
}

void TrainerComponent::Render() const
{
	if (m_ShowItemPrompt)
	{
		RenderItemPrompt();
	}

	RenderRoute();
}

PokemonComponent* TrainerComponent::GetActivePokemon() const
{
	return m_ActivePokemon;
}

void TrainerComponent::HealActivePokemon()
{
	m_ActivePokemon->SetCurrentHealth(m_ActivePokemon->GetStats().Health);
}

void TrainerComponent::RenderItemPrompt() const
{
	constexpr glm::vec2 boxscale{ 2.0f, 1.5f };
	constexpr glm::vec2 textPromptScale{ 1.0f };
	constexpr glm::ivec2 whiteSpace{ 20, 10 };

	const glm::ivec2 boxSize{ static_cast<int>(m_InfoBoxTexture->GetSize().x * boxscale.x), static_cast<int>(m_InfoBoxTexture->GetSize().y * boxscale.y) };
	const glm::ivec2 boxPosition{ Engine::GetWindowSize().x / 2, boxSize.y / 2 };
	Renderer::Instance()->RenderTexture(*m_InfoBoxTexture, Transform{ boxPosition, 0, boxscale });

	const glm::ivec2 itemPromptSize{ static_cast<int>(m_ItemPromptText->GetTexture()->GetSize().x * textPromptScale.x), static_cast<int>(m_ItemPromptText->GetTexture()->GetSize().y * textPromptScale.y)};
	const glm::ivec2 itemPromptPosition{ boxPosition.x - (boxSize.x / 2) + (itemPromptSize.x / 2) + whiteSpace.x, boxPosition.y + (boxSize.y / 2) - (itemPromptSize.y / 2) - whiteSpace.y};
	Renderer::Instance()->RenderTexture(*m_ItemPromptText->GetTexture(), Transform{ itemPromptPosition, 0, glm::vec2{ textPromptScale } });

	const glm::ivec2 inventoryPromptSize{ static_cast<int>(m_InventoryPromptText->GetTexture()->GetSize().x * textPromptScale.x), static_cast<int>(m_InventoryPromptText->GetTexture()->GetSize().y * textPromptScale.y) };
	const glm::ivec2 inventoryPromptPosition{ boxPosition.x - (boxSize.x / 2) + (inventoryPromptSize.x / 2) + whiteSpace.x, boxPosition.y - (boxSize.y / 2) + (inventoryPromptSize.y / 2) + whiteSpace.y };
	Renderer::Instance()->RenderTexture(*m_InventoryPromptText->GetTexture(), Transform{ inventoryPromptPosition, 0, glm::vec2{ textPromptScale } });
}

void TrainerComponent::SwitchItemPrompt(bool enable)
{
	if (enable)
	{
		m_ShowItemPrompt = true;
		m_MovementComponent->SetStatus(ControllableObject::Status::Disabled);
	}
	else
	{
		m_ShowItemPrompt = false;
		m_MovementComponent->SetStatus(ControllableObject::Status::Enabled);
	}
}

void TrainerComponent::ConfirmPickup()
{
	if (m_ShowItemPrompt)
	{
		SwitchItemPrompt(false);
	}
}

void TrainerComponent::IncreaseScore(uint16_t amount)
{
	m_Score += amount;
	m_RouteText->SetText(std::format("Route {}", m_Score));
}

void TrainerComponent::Reset()
{
	m_Score = 1;
	m_RouteText->SetText(std::format("Route {}", m_Score));
	m_ActivePokemon->Reset();
	m_ActivePokemon->SetLevel(3);
	for (size_t index{ 0 }; index < static_cast<size_t>(ItemType::Count); ++index)
	{
		m_Inventory.at(index) = 0;
	}
}

bool TrainerComponent::TryUseItem(const ItemType item)
{
	bool canUseItem{ m_Inventory.at(static_cast<size_t>(item)) > 0 };

	// There is still item's left of the type
	if (canUseItem)
	{
		const uint16_t itemsLeft{ --m_Inventory.at(static_cast<size_t>(item)) };
		const std::string pokemonName{ m_ActivePokemon->GetNameText()->GetText() };
		SwitchItemPrompt(true);

		switch (item)
		{
		case ItemType::FullRestore:
			HealActivePokemon();
			m_ItemPromptText->SetText(std::format("Restored {}'s health back to full!", pokemonName));
			if (itemsLeft == 0) m_InventoryPromptText->SetText("We Have no Full Restores left.");
			else if(itemsLeft == 1) m_InventoryPromptText->SetText("We have only 1 Full Restore left.");
			else m_InventoryPromptText->SetText(std::format("We Have {} Full Restores left.", itemsLeft));
			break;
		case ItemType::RareCandy:
			GetActivePokemon()->LevelUp();
			m_ItemPromptText->SetText(std::format("Increased {}'s level to Lv {}!", pokemonName, m_ActivePokemon->GetLevel()));
			if (itemsLeft == 0) m_InventoryPromptText->SetText("We Have no Rare Candies left.");
			else if (itemsLeft == 1) m_InventoryPromptText->SetText("We have only 1 Rare Candy left.");
			else m_InventoryPromptText->SetText(std::format("We Have {} Rare Candies left.", itemsLeft));
			break;
		default:
			break;
		}
	}

	return canUseItem;
}

void TrainerComponent::PickupItem(const ItemType item)
{
	const uint16_t itemsLeft{ ++m_Inventory.at(static_cast<size_t>(item)) };

	switch (item)
	{
	case ItemType::FullRestore:
		m_ItemPromptText->SetText("Picked up a Full Restore!");
		 if(itemsLeft == 1) m_InventoryPromptText->SetText("We now have 1 Full Restore.");
		 else m_InventoryPromptText->SetText(std::format("We now have {} Full Restores.", itemsLeft));
		break;
	case ItemType::RareCandy:
		m_ItemPromptText->SetText("Picked up a Rare Candy!");
		if (itemsLeft == 1) m_InventoryPromptText->SetText("We now have 1 Rare Candy.");
		else m_InventoryPromptText->SetText(std::format("We now have {} Rare Candies.", itemsLeft));
		break;
	}
	
	SwitchItemPrompt(true);
}

PokemonComponent* TrainerComponent::CreateStartPokemon() const
{
	PODPokemon pokemon{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetPokemon(6) };
	GameObject* pokemonObject{ GetOwner()->GetScene()->CreateGameObject(std::format("Trainer's {}", pokemon.Name)) };
	PokemonComponent* pokemonComponent{ pokemonObject->CreateComponent<PokemonComponent>(pokemon, this) };
	pokemonComponent->SetLevel(3);

	return pokemonComponent;
}

void TrainerComponent::RenderRoute() const
{
	const glm::ivec2 routePosition{ 900.0f, 450.0f };

	Renderer::Instance()->RenderTexture(*m_RouteTexture, Transform{ routePosition, 0, glm::vec2{ 1.0f } });
	Renderer::Instance()->RenderTexture(*m_RouteText->GetTexture(), Transform{ routePosition, 0, glm::vec2{ 1.0f } });
}