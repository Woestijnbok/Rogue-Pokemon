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
	m_ActivePokemon{ CreateStartPokemon() },
	m_ShowPickupPrompt{ false },
	m_MovementComponent{ owner->GetComponent<MovementComponent>() },
	m_InfoBoxTexture{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<BattleManagerComponent>()->GetInfoBoxTexture() },
	m_RouteTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Route Sign.png") },
	m_PickupText{ new Text{ "is back to full health!", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_RouteText{ new Text{ "Route 1", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 25), Color::Black } },
	m_Score{ 1 }
{
	SetRenderOrder(RenderOrder::UI);
}

void TrainerComponent::Render() const
{
	if (m_ShowPickupPrompt)
	{
		const glm::vec2 scale{ 2.0f, 1.5f };
		const int whitespace{ 5 };

		const glm::ivec2 boxPosition{ Engine::GetWindowSize().x / 2, m_InfoBoxTexture->GetSize().y / 2 * scale.y };
		Renderer::Instance()->RenderTexture(*m_InfoBoxTexture, Transform{ boxPosition, 0, scale });

		const glm::ivec2 namePosition{ 310, 35 };
		Renderer::Instance()->RenderTexture(*m_ActivePokemon->GetNameText()->GetTexture(), Transform{namePosition, 0, glm::vec2{ 1.0f }});

		const glm::ivec2 textPosition{ namePosition.x + (m_ActivePokemon->GetNameText()->GetTexture()->GetSize().x / 2) + whitespace + (m_PickupText->GetTexture()->GetSize().x / 2), namePosition.y};
		Renderer::Instance()->RenderTexture(*m_PickupText->GetTexture(), Transform{textPosition, 0, glm::vec2{1.0f}});
	}

	RenderRoute();
}

PokemonComponent* TrainerComponent::GetActivePokemon() const
{
	return m_ActivePokemon;
}

bool TrainerComponent::CanPickupItem() const
{
	return m_ActivePokemon->GetCurrentHealth() != m_ActivePokemon->GetStats().Health;
}

void TrainerComponent::HealActivePokemon()
{
	m_ActivePokemon->SetCurrentHealth(m_ActivePokemon->GetStats().Health);
	m_ShowPickupPrompt = true;
	m_MovementComponent->SetStatus(ControllableObject::Status::Disabled);
}

void TrainerComponent::ConfirmPickup()
{
	if (m_ShowPickupPrompt)
	{
		m_ShowPickupPrompt = false;
		m_MovementComponent->SetStatus(ControllableObject::Status::Enabled);
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