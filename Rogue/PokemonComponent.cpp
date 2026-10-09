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

// Components
#include "TrainerComponent.h"

// Other
#include "Pokedex.hpp"
#include "Helpers.h"

using namespace Minigin;

PokemonComponent::PokemonComponent(GameObject* owner, const PODPokemon& pokemon, TrainerComponent const * trainer) :
	Component{ owner },
	m_Stats{},
	m_Moves
	{
		Move{ pokemon.Moves[0], ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ pokemon.Moves[1], ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ pokemon.Moves[2], ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) },
		Move{ pokemon.Moves[3], ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30) }
	},
	m_Name{ new Text{ pokemon.Name, ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_LevelText{ new Text{ "Lv ", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_Texture{ Renderer::Instance()->CreateTexture(GetPokemonTexturePath(pokemon.Name, false)) },
	m_Trainer{ trainer }
{
	
}

PokemonComponent::PokemonComponent(Minigin::GameObject* owner, const PODPokemon& pokemon) :
	Component{ owner },
	m_Stats{},
	m_Moves
	{
		Move{ pokemon.Moves[0], ResourceManager::Instance()->GetFont("Emerald", 30) },
		Move{ pokemon.Moves[1], ResourceManager::Instance()->GetFont("Emerald", 30) },
		Move{ pokemon.Moves[2], ResourceManager::Instance()->GetFont("Emerald", 30) },
		Move{ pokemon.Moves[3], ResourceManager::Instance()->GetFont("Emerald", 30) }
	},
	m_Name{ new Text{ pokemon.Name, ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_LevelText{ new Text{ "Lv ", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
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
	return static_cast<float>(m_Stats.CurrentHealth) / static_cast<float>(m_Stats.MaxHealth);
}

bool PokemonComponent::IsDead() const
{
	return m_Stats.CurrentHealth <= 0;
}

TrainerComponent const * PokemonComponent::GetTrainer() const
{
	return  m_Trainer;
}

void PokemonComponent::SetLevel(uint16_t level)
{
	m_Stats.Level = std::clamp(level, static_cast<uint16_t>(1), static_cast<uint16_t>(100));

	m_LevelText->SetText(std::format("Lv {}", m_Stats.Level));
}

PokemonStats& PokemonComponent::GetStats()
{
	return m_Stats;
}

const std::array<Move, 4>& PokemonComponent::GetMoves() const
{
	return m_Moves;
}