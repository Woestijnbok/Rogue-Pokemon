#include "TrainerComponent.h"

// Core
#include "GameObject.h"

// Components
#include "PokemonComponent.h"

// Other
#include "Helpers.h"

using namespace Minigin;

TrainerComponent::TrainerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_ActivePokemon{ ReadPokemon(6) } // Blaziken pokedex index
{
	
}

PokemonComponent* TrainerComponent::GetActivePokemon() const
{
	return m_ActivePokemon;
}