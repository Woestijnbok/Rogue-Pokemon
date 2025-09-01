#include "BattleManagerComponent.h"

// Libraries
#include <cassert>

// Core
#include "Engine.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h"
#include "Text.h"
#include "Scene.h"

// Components
#include "PokemonComponent.h"
#include "TrainerComponent.h"

// Other
#include "Helpers.h"
#include "Pokedex.hpp"

using namespace Minigin;

BattleManagerComponent::BattleManagerComponent(GameObject* owner) :
	Component{ owner },
	m_CurrentBattle{ nullptr, nullptr },
	m_OnBattleStarted{},
	m_OnBattleFinished{},
	m_BattleBackground{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Battle Background.png") },
	m_TrainerCloud{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Trainer Cloud.png") },
	m_EnemyCloud{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Enemy Cloud.png") },
	m_InfoBox{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Info Box.png") },
	m_MoveBox{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Move Box.png") },
	m_RandomDevice{},
	m_RandomEngine{ m_RandomDevice() },
	m_ChanceDistribution{ 0.0f, 100.0f },
	m_CommonDistribution{ 1, POKEDEX_LEGENDARY_START - 1 },
	m_LegendaryDistribution{ POKEDEX_LEGENDARY_START, POKEDEX_COUNT },
	m_LegendaryChance{ 5 }
{
	assert(m_BattleBackground.get());
}

void BattleManagerComponent::Render() const
{
	RenderBackground();
	RenderPokemons();
	RenderTrainerCloud();
	RenderEnemyCloud();
	RenderMoveSelect();
}

void BattleManagerComponent::MakeBattle(TrainerComponent* trainer)
{
	uint8_t enemyPokemonIndex{ 0 };
	if (m_ChanceDistribution(m_RandomEngine) <= m_LegendaryChance)
	{
		enemyPokemonIndex = static_cast<uint8_t>(m_LegendaryDistribution(m_RandomEngine));
	}
	else
	{
		enemyPokemonIndex = static_cast<uint8_t>(m_CommonDistribution(m_RandomEngine));
	}

	PODPokemon pokemon{};
	ReadPokemon(pokemon, enemyPokemonIndex);

	GameObject* wildPokemonObject{ GetOwner()->GetScene()->CreateGameObject(std::format("Wild {}", pokemon.Name)) };
	PokemonComponent* wildPokemonComponent{ wildPokemonObject->CreateComponent<PokemonComponent>(pokemon) };

	StartBattle(trainer->GetActivePokemon(), wildPokemonComponent);
}

void BattleManagerComponent::EndBattle()
{
	m_CurrentBattle.first = nullptr;
	m_CurrentBattle.second->GetOwner()->SetStatus(ControllableObject::Status::Destroyed);
	m_CurrentBattle.second = nullptr;

	m_OnBattleFinished.Notify();
}

bool BattleManagerComponent::InBattle() const
{
	return m_CurrentBattle.second != nullptr;
}

Minigin::Subject<>& BattleManagerComponent::OnBattleStarted()
{
	return m_OnBattleStarted;
}

Minigin::Subject<>& BattleManagerComponent::OnBattleFinished()
{
	return m_OnBattleFinished;
}

void BattleManagerComponent::StartBattle(PokemonComponent* trainer, PokemonComponent* enemy)
{
	m_CurrentBattle.first = trainer;
	m_CurrentBattle.second = enemy;

	m_OnBattleStarted.Notify();
}

void BattleManagerComponent::RenderBackground() const
{
	const glm::ivec2 windowSize{ Engine::GetWindowSize() };
	const glm::ivec2 textureSize{ m_BattleBackground->GetSize() };

	const Transform transform{ windowSize / 2 , 0, glm::vec2{ windowSize.x / float(textureSize.x), windowSize.y / float(textureSize.y) } };
	Renderer::Instance()->RenderTexture(*m_BattleBackground, transform);
}

void BattleManagerComponent::RenderPokemons() const
{
	const Transform trainerTransform{ glm::ivec2{ 200, 95 } , 0, glm::vec2{ 3.0f } };
	Renderer::Instance()->RenderTexture(*m_CurrentBattle.first->GetTexture(), trainerTransform);

	const Transform enemyTransform{ glm::ivec2{ 700, 290 }, 0, glm::vec2{ 3.0f } };
	Renderer::Instance()->RenderTexture(*m_CurrentBattle.second->GetTexture(), enemyTransform);
}

void BattleManagerComponent::RenderTrainerCloud() const
{
	const Transform cloudTransform{ glm::ivec2{ 500, 150 }, 0, glm::vec2{ 3.0f } };
	Renderer::Instance()->RenderTexture(*m_TrainerCloud, cloudTransform);

	const int nameWidth{ m_CurrentBattle.first->GetNameText()->GetTexture()->GetSize().x };
	const Transform nameTransform{ glm::ivec2{ 400 + (nameWidth / 2), 170 }, 0, glm::vec2{ 1.0f } };
	Renderer::Instance()->RenderText(*m_CurrentBattle.first->GetNameText(), nameTransform);

	const int levelWidth{ m_CurrentBattle.first->GetLevelText()->GetTexture()->GetSize().x };
	const Transform levelTransform{ glm::ivec2{ 620 - (levelWidth / 2), 170 }, 0, glm::vec2{ 1.0f } };
	Renderer::Instance()->RenderText(*m_CurrentBattle.first->GetLevelText(), levelTransform);
	//
}

void BattleManagerComponent::RenderEnemyCloud() const
{
	const Transform cloudTransform{ glm::ivec2{ 350, 350 }, 0, glm::vec2{ 3.0f } };
	Renderer::Instance()->RenderTexture(*m_TrainerCloud, cloudTransform);

	const int nameWidth{ m_CurrentBattle.second->GetNameText()->GetTexture()->GetSize().x };
	const Transform nameTransform{ glm::ivec2{ 250 + (nameWidth / 2), 370}, 0, glm::vec2{1.0f}};
	Renderer::Instance()->RenderText(*m_CurrentBattle.second->GetNameText(), nameTransform);

	const int levelWidth{ m_CurrentBattle.second->GetLevelText()->GetTexture()->GetSize().x };
	const Transform levelTransform{ glm::ivec2{ 470 - (levelWidth / 2), 370 }, 0, glm::vec2{ 1.0f } };
	Renderer::Instance()->RenderText(*m_CurrentBattle.second->GetLevelText(), levelTransform);
}

void BattleManagerComponent::RenderMoveSelect() const
{
	const Transform transform{ glm::ivec2{ 745, 41 }, 0, glm::vec2{ 1.8f } };
	Renderer::Instance()->RenderTexture(*m_MoveBox, transform);
}