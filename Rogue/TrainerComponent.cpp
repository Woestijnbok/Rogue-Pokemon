#include "TrainerComponent.h"

// Libraries
#include <format>

// Core
#include "GameObject.h"
#include "Scene.h"

// Components
#include "PokemonComponent.h"

// Other
#include "Helpers.h"
#include "Pokedex.hpp"

using namespace Minigin;

TrainerComponent::TrainerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_ActivePokemon{ CreateStartPokemon() }
{
	
}

PokemonComponent* TrainerComponent::GetActivePokemon() const
{
	return m_ActivePokemon;
}

PokemonComponent* TrainerComponent::CreateStartPokemon() const
{
	// TODO: Create option to choose now it's always blaziken
	PODPokemon pokemon{};
	ReadPokemon(pokemon, 6);

	GameObject* pokemonObject{ GetOwner()->GetScene()->CreateGameObject(std::format("Trainer's {}", pokemon.Name)) };
	PokemonComponent* pokemonComponent{ pokemonObject->CreateComponent<PokemonComponent>(pokemon, this) };

	return pokemonComponent;
}