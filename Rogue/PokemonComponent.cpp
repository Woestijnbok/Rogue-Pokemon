#include "PokemonComponent.h"

// Libraries
#include <format>
#include <algorithm>

// Core
#include "GameObject.h"
#include "Texture.h"
#include "Text.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Color.h"
#include "SceneManager.h"

// Components
#include "TrainerComponent.h"
#include "PokedexComponent.h"

// Other
#include "Pokedex.hpp"
#include "Helpers.h"

using namespace Minigin;

PokemonComponent::PokemonComponent(GameObject* owner, const PODPokemon& pokemon, TrainerComponent const * trainer) :
	Component{ owner },
	m_ID{ pokemon.ID },
	m_Level{ 1 },
	m_Experience{ 0 },
	m_Stats{ pokemon },
	m_CurrentHealth{ pokemon.Health },
	m_Types{ pokemon.Types[0], pokemon.Types[1] },
	m_Moves
	{
		Move{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetMove(pokemon.Moves[0]), ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetMove(pokemon.Moves[1]), ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetMove(pokemon.Moves[2]), ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetMove(pokemon.Moves[3]), ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) }
	},
	m_Name{ new Text{ pokemon.Name, ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_LevelText{ new Text{ "Lv", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_Texture{ Renderer::Instance()->CreateTexture(GetPokemonTexturePath(pokemon.Name, false)) },
	m_Trainer{ trainer }
{
	
}

PokemonComponent::PokemonComponent(Minigin::GameObject* owner, const PODPokemon& pokemon) :
	Component{ owner },
	m_ID{ pokemon.ID },
	m_Level{ 1 },
	m_Stats{ pokemon },
	m_CurrentHealth{ pokemon.Health },
	m_Types{ pokemon.Types[0], pokemon.Types[1] },
	m_Moves
	{
		Move{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetMove(pokemon.Moves[0]), ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetMove(pokemon.Moves[1]), ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetMove(pokemon.Moves[2]), ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetMove(pokemon.Moves[3]), ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) }
	},
	m_Name{ new Text{ pokemon.Name, ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_LevelText{ new Text{ "Lv", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_Texture{ Renderer::Instance()->CreateTexture(GetPokemonTexturePath(pokemon.Name, true)) },
	m_Trainer{ nullptr }
{
	SetLevel(100);
}

Text * PokemonComponent::GetNameText() const
{
	return m_Name.get();
}

Minigin::Text* PokemonComponent::GetLevelText() const
{
	return m_LevelText.get();
}

Texture const * PokemonComponent::GetTexture() const
{
	return m_Texture.get();
}

bool PokemonComponent::IsWild() const
{
	return m_Trainer == nullptr;
}

float PokemonComponent::GetHealthPercentage() const
{
	return static_cast<float>(m_CurrentHealth) / static_cast<float>(m_Stats.Health);
}

bool PokemonComponent::IsDead() const
{
	return m_CurrentHealth <= 0;
}

TrainerComponent const * PokemonComponent::GetTrainer() const
{
	return  m_Trainer;
}

void PokemonComponent::SetLevel(int level)
{
	if (m_Level != level)
	{
		int levelDifference{ static_cast<int>(level) - static_cast<int>(m_Level) };
		m_CurrentHealth = std::max(0, m_CurrentHealth + levelDifference);
		m_Stats += levelDifference;
		m_Level = std::clamp(level, 1, 100);
		m_LevelText->SetText(std::format("Lv {}", m_Level));
	}
}

void PokemonComponent::LevelUp()
{
	m_Level = std::clamp(m_Level + 1, 1, 100);
	m_LevelText->SetText(std::format("Lv {}", m_Level));
	++m_Stats;
}

size_t PokemonComponent::GetPokedexID() const
{
	return m_ID;
}

bool PokemonComponent::IsOfElement(size_t type) const
{
	if (type <= 0 or type > POKEDEX_ELEMENT_COUNT)
	{
		throw std::runtime_error{"PokemonComponent::IsOfElement(...) invalid element type index"};
	}

	return std::ranges::any_of(m_Types, [type](uint8_t element) -> bool { return element == type; });
}

std::pair<uint8_t, uint8_t> PokemonComponent::GetElements() const
{
	return std::make_pair(m_Types.at(0), m_Types.at(1));
}

int PokemonComponent::GetLevel() const
{
	return m_Level;
}

void PokemonComponent::GainExperience(int experience)
{
	if (experience <= 0)
	{
		throw std::runtime_error{"PokemonComponent::GainExperience(...) - Should not get negative experience or 0"};
	}

	m_Experience += experience;

	// Keep leveling up until the gained experience isn't enough to go to the next level.
	while (m_Experience >= m_Level)
	{
		m_Experience -= m_Level;
		LevelUp();
	}
}

const PokemonStats& PokemonComponent::GetStats() const
{
	return m_Stats;
}

int PokemonComponent::GetCurrentHealth() const
{
	return m_CurrentHealth;
}

void PokemonComponent::SetCurrentHealth(int health)
{
	m_CurrentHealth = std::clamp(health, 0, m_Stats.Health);
}

void PokemonComponent::TakeDamage(int damage)
{
	SetCurrentHealth(std::max(0, m_CurrentHealth - damage));
}

const std::array<Move, 4>& PokemonComponent::GetMoves() const
{
	return m_Moves;
}

void PokemonComponent::Reset()
{
	SetCurrentHealth(m_Stats.Health);
	SetLevel(1);
	m_Experience = 0;
}