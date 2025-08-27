#include "BattleManagerComponent.h"

// Libraries
#include <cassert>

// Core
#include "Engine.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h" 

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
	m_EnemyPokemonTexture{},
	m_TrainerPokemonTexture{},
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
	const glm::ivec2 windowSize{ Engine::GetWindowSize() };
	const glm::ivec2 textureSize{ m_BattleBackground->GetSize() };

	Transform transform{ windowSize / 2 , 0, glm::vec2{ windowSize.x / float(textureSize.x), windowSize.y / float(textureSize.y) } };
	Renderer::Instance()->RenderTexture(*m_BattleBackground, transform);

	transform.SetScale(glm::vec2{ 3.0f });
	transform.SetPosition(glm::ivec2{ 200, 100 });
	Renderer::Instance()->RenderTexture(*m_TrainerPokemonTexture, transform);

	transform.SetPosition(glm::ivec2{ 700, 350 });
	Renderer::Instance()->RenderTexture(*m_EnemyPokemonTexture, transform);
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

	StartBattle(trainer->GetActivePokemon(), ReadPokemon(enemyPokemonIndex));
}

void BattleManagerComponent::EndBattle()
{
	m_CurrentBattle.second->GetOwner()->SetStatus(ControllableObject::Status::Destroyed);
	m_CurrentBattle.second = nullptr;
	m_EnemyPokemonTexture.reset();

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
	if (m_CurrentBattle.first != trainer)
	{
		m_CurrentBattle.first = trainer;
		m_TrainerPokemonTexture.reset(Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / std::format("Pokemon/{}.png", trainer->GetName())));
	}
	
	m_CurrentBattle.second = enemy;
	m_EnemyPokemonTexture.reset(Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / std::format("Pokemon/{}.png", enemy->GetName())));

	m_OnBattleStarted.Notify();
}