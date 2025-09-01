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
	m_NameText{ new Text{ pokemon.Name, ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_LevelText{ new Text{ "Lv 100", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_Texture{ Renderer::Instance()->CreateTexture(GetPokemonTexturePath(pokemon.Name, false)) },
	m_Moves
	{ 
		Move{ pokemon.Moves[0].Name, pokemon.Moves[0].Power }, 
		Move{ pokemon.Moves[1].Name, pokemon.Moves[1].Power },
		Move{ pokemon.Moves[2].Name, pokemon.Moves[2].Power },
		Move{ pokemon.Moves[3].Name, pokemon.Moves[3].Power }
	},
	m_Trainer{ trainer }
{
	
}

PokemonComponent::PokemonComponent(Minigin::GameObject* owner, const PODPokemon& pokemon) :
	Component{ owner },
	m_NameText{ new Text{ pokemon.Name, ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_LevelText{ new Text{ "Lv 100", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_Texture{ Renderer::Instance()->CreateTexture(GetPokemonTexturePath(pokemon.Name, true)) },
	m_Moves
	{
		Move{ pokemon.Moves[0].Name, pokemon.Moves[0].Power },
		Move{ pokemon.Moves[1].Name, pokemon.Moves[1].Power },
		Move{ pokemon.Moves[2].Name, pokemon.Moves[2].Power },
		Move{ pokemon.Moves[3].Name, pokemon.Moves[3].Power }
	},
	m_Trainer{}
{
	
}

const std::string& PokemonComponent::GetName() const
{
	return m_NameText->GetText();
}

Text * PokemonComponent::GetNameText() const
{
	return m_NameText.get();
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
	m_LevelText->SetText(std::format("Lv {}", level));
}

uint8_t PokemonComponent::GetLevel() const
{
	return static_cast<uint8_t>(std::stoi(m_LevelText->GetText().substr(3)));
}