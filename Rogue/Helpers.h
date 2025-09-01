#pragma once

// Libraries
#include <cstdint>
#include <filesystem>

namespace Minigin
{
	class SpriteComponent;
	class GameObject;
}

struct PODPokemon;
class TileManagerComponent;
class MovementComponent;

/*
* Reads pokemon information from Pokedex.bin (binary pokedex file).
* 
* @param pokedexIndex: The pokedex index that will be used to read the correct pokemon in the pokedex file.
*/
void ReadPokemon(PODPokemon& pokemon, uint8_t pokedexIndex);

/*
* Makes a sprite component and adds all the trainer sprites.
* 
* @param object: The owning game object.
* @param tileManager: The tile manager used to get render scale.
* 
* @return The created sprite component.
*/
Minigin::SpriteComponent* MakeTrainerSprites(Minigin::GameObject* object, TileManagerComponent* tileManager, MovementComponent* movementComponent);

/*
* Binds the changes of trainer sprites based on movement component events.
* 
* @param spriteComponent: Sprite component responsible for trainer sprites.
* @param movementComponent: The movement component to base changes on.
*/
void ConnectSpritesToMovement(Minigin::SpriteComponent* spriteComponent, MovementComponent* movementComponent);

/*
* Gets the file path for the correct texture of the pokemon.
*
* @param name: The name of the pokemon.
* @param isWild: Wether or not the pokemon is wild, if  so front texture else back texture.
*
* @return The file path to the texture of the pokemon.
*/
std::filesystem::path GetPokemonTexturePath(const std::string& name, bool isWild);