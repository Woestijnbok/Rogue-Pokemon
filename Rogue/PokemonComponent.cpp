#include "PokemonComponent.h"

// Libraries
#include <format>

// Core
#include "GameObject.h"
#include "Texture.h"
#include "Text.h"
#include "ResourceManager.h"
#include "Renderer.h"

// Components
#include "TrainerComponent.h"

// Other
#include "Pokedex.hpp"
#include "Helpers.h"

using namespace Minigin;

PokemonComponent::PokemonComponent(GameObject* owner, const PODPokemon& pokemon, TrainerComponent* trainer) :
	Component{ owner },
	m_Name{ new Text{ pokemon.Name, ResourceManager::Instance()->GetOrLoadFont("Emerald.FON", "Emerald", 20) } },
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
	m_Name{  },
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
	return m_Name->GetText();
}

Text const * PokemonComponent::GetText() const
{
	return m_Name.get();
}

Texture const * PokemonComponent::GetTexture() const
{
	return m_Texture.get();
}

bool PokemonComponent::IsWild() const
{
	return !m_Trainer.has_value();
}