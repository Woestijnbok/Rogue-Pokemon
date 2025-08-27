#include "PokemonComponent.h"

// Core
#include "GameObject.h"

// Other
#include "Pokedex.hpp"

using namespace Minigin;

PokemonComponent::PokemonComponent(GameObject* owner, const PODPokemon& pokemon) :
	Component{ owner },
	m_Name{ pokemon.Name },
	m_Moves
	{ 
		Move{ pokemon.Moves[0].Name, pokemon.Moves[0].Power }, 
		Move{ pokemon.Moves[1].Name, pokemon.Moves[1].Power },
		Move{ pokemon.Moves[2].Name, pokemon.Moves[2].Power },
		Move{ pokemon.Moves[3].Name, pokemon.Moves[3].Power }
	}
{
	
}

const std::string& PokemonComponent::GetName() const
{
	return m_Name;
}