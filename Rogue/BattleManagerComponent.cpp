#include <filesystem>
#include <fstream>
#include <cassert>

#include "BattleManagerComponent.h"
#include "GameObject.h"
#include "PokemonComponent.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Pokedex.hpp"
#include "Renderer.h"
#include "Engine.h"
#include "Texture.h" 

using namespace Minigin;

BattleManagerComponent::BattleManagerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_CurrentBattle{ nullptr, nullptr },
	m_OnBattleStarted{},
	m_OnBattleFinished{},
	m_BattleBackground{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Battle Background.png") },
	m_EnemyPokemonTexture{},
	m_TrainerPokemonTexture{},
	m_RandomDevice{},
	m_RandomEngine{ m_RandomDevice() },
	m_RandomDistribution{ 1, POKEDEX_COUNT }
{
	assert(m_BattleBackground.get());
}

void BattleManagerComponent::Render() const
{
	const glm::ivec2 windowSize{ Engine::GetWindowSize() };
	const glm::ivec2 textureSize{ m_BattleBackground->GetSize() };

	Transform transform{ windowSize / 2 , 0, glm::vec2{ windowSize.x / float(textureSize.x), windowSize.y / float(textureSize.y) } };
	Renderer::Instance()->RenderTexture(*m_BattleBackground, transform);

	transform.SetScale(glm::vec2{ 1.0f });
	transform.SetPosition(glm::ivec2{ 50, 50 });
	Renderer::Instance()->RenderTexture(*m_TrainerPokemonTexture, transform);

	transform.SetPosition(glm::ivec2{ 700, 350 });
	Renderer::Instance()->RenderTexture(*m_EnemyPokemonTexture, transform);
}

void BattleManagerComponent::MakeBattle(TrainerComponent* trainer)
{
	trainer;
	StartBattle(ReadPokemon(static_cast<uint8_t>(m_RandomDistribution(m_RandomEngine))), ReadPokemon(static_cast<uint8_t>(m_RandomDistribution(m_RandomEngine))));
}

void BattleManagerComponent::EndBattle()
{
	m_OnBattleFinished.Notify();

	m_CurrentBattle.first->GetOwner()->SetStatus(ControllableObject::Status::Destroyed);
	m_CurrentBattle.second->GetOwner()->SetStatus(ControllableObject::Status::Destroyed);
	m_TrainerPokemonTexture.reset();
	m_EnemyPokemonTexture.reset();
}

Minigin::Subject<>& BattleManagerComponent::OnBattleStarted()
{
	return m_OnBattleStarted;
}

Minigin::Subject<>& BattleManagerComponent::OnBattleFinished()
{
	return m_OnBattleFinished;
}

PokemonComponent* BattleManagerComponent::ReadPokemon(uint8_t pokedexIndex) const
{
	const std::filesystem::path path{ ResourceManager::Instance()->GetFileRootPath() / "Pokedex.bin" };
	assert(std::filesystem::exists(path));

	std::ifstream file{ path, std::ios::binary };
	assert(file.is_open());

	file.seekg(sizeof(PODPokemon) * (pokedexIndex - 1));

	PODPokemon pokemon{};
	file.read(reinterpret_cast<char*>(&pokemon), sizeof(PODPokemon));

	Scene* battleScene{ SceneManager::Instance()->GetScene("Battle") };
	GameObject* pokemonObject{ battleScene->CreateGameObject(pokemon.Name) };
	PokemonComponent* pokemonComponent{ pokemonObject->CreateComponent<PokemonComponent>(pokemon) };

	return pokemonComponent;
}

void BattleManagerComponent::StartBattle(PokemonComponent* player, PokemonComponent* enemy)
{
	m_CurrentBattle.first = player;
	m_CurrentBattle.second = enemy;

	m_TrainerPokemonTexture.reset(Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / std::format("Pokemon/{}.png", player->GetName())));
	m_EnemyPokemonTexture.reset(Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / std::format("Pokemon/{}.png", enemy->GetName())));

	m_OnBattleStarted.Notify();
}