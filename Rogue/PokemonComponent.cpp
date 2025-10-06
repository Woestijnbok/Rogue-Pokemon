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

PokemonComponent::PokemonComponent(GameObject* owner, const PODPokemon& pokemon, TrainerComponent const* trainer) :
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
	SetLevel(100);
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
	m_Trainer{}
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
	return !m_Trainer.has_value();
}

void PokemonComponent::SetLevel(uint8_t level)
{
	m_Stats.Level = level;
	m_LevelText->SetText(std::format("Lv {}", level));
}

float PokemonComponent::GetHealthPercentage() const
{
	return static_cast<float>(m_Stats.CurrentHealth) / static_cast<float>(m_Stats.MaxHealth);
}

PokemonStats& PokemonComponent::GetStats()
{
	return m_Stats;
}

const std::array<Move, 4>& PokemonComponent::GetMoves() const
{
	return m_Moves;
}