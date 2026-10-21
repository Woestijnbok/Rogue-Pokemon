#include "PokemonComponent.h"

// Libraries
#include <format>

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
	m_CurrentHealth{ pokemon.Health },
	m_Types{ pokemon.Types[0], pokemon.Types[1] },
	m_Stats{ pokemon },
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
	m_CurrentHealth{ pokemon.Health },
	m_Types{ pokemon.Types[0], pokemon.Types[1] },
	m_Stats{ pokemon },
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

void PokemonComponent::SetLevel(uint8_t level)
{
	m_Level = std::clamp(level, static_cast<uint8_t>(1), static_cast<uint8_t>(100));
	m_LevelText->SetText(std::format("Lv {}", m_Level));

	const PODPokemon& pokedexEntry{ SceneManager::Instance()->GetScene("Battle")->GetGameObject("Manager")->GetComponent<PokedexComponent>()->GetPokemon(m_ID) };
	if (m_Level > 1)
	{
		m_Stats = PokemonStats{ pokedexEntry } + static_cast<uint16_t>(level - 1);
	}
	else
	{
		m_Stats = PokemonStats{ pokedexEntry };
	}
}

void PokemonComponent::LevelUp()
{
	m_Level = std::clamp(static_cast<uint8_t>(m_Level + 1), static_cast<uint8_t>(1), static_cast<uint8_t>(100));
	m_LevelText->SetText(std::format("Lv {}", m_Level));
	++m_Stats;
}

size_t PokemonComponent::GetPokedexID() const
{
	return m_ID;
}

bool PokemonComponent::IsOfElement(uint8_t type) const
{
	if (type <= 0 or type >= POKEDEX_ELEMENT_COUNT)
	{
		throw std::runtime_error{"PokemonComponent::IsOfElement(...) invalid element type index"};
	}

	return std::ranges::any_of(m_Types, [type](uint8_t element) -> bool { return element == type; });
}

uint8_t PokemonComponent::GetLevel() const
{
	return m_Level;
}

void PokemonComponent::GainExperience(uint8_t experience)
{
	// Should not be possibly to overflow the uint8_t since the max experience is 100 for a lvl 100 pokemon
	m_Experience += experience;

	// To go to lvl n from lvl m the pokemon needs m xp.
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

uint16_t PokemonComponent::GetCurrentHealth() const
{
	return m_CurrentHealth;
}

void PokemonComponent::SetCurrentHealth(uint16_t health)
{
	m_CurrentHealth = std::clamp(health, static_cast<uint16_t>(0), m_Stats.Health);
}

void PokemonComponent::TakeDamage(uint16_t damage)
{
	if (m_CurrentHealth < damage)
	{
		SetCurrentHealth(0);
	}
	else
	{
		SetCurrentHealth(m_CurrentHealth - damage);
	}
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