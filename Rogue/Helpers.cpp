#include "Helpers.h"

// Libraries
#include <filesystem>
#include <fstream>

// Core
#include "ResourceManager.h"
#include "SceneManager.h"

// Components
#include "PokemonComponent.h"

// Other
#include "Pokedex.hpp"

using namespace Minigin;

PokemonComponent* ReadPokemon(uint8_t pokedexIndex)
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