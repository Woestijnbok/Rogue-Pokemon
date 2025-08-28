#pragma once

// Libraries
#include <cstdint>

namespace Minigin
{
	class SpriteComponent;
	class GameObject;
}

class PokemonComponent;
class TileManagerComponent;
class MovementComponent;

/*
* Reads pokemon information from Pokedex.bin (binary pokedex file) to create pokemon component.
* 
* @param pokedexIndex: The pokedex index that will be used to read the correct pokemon in the pokedex file.
* 
* @return The pokemon component based on the given pokedex information.
*/
PokemonComponent* ReadPokemon(uint8_t pokedexIndex);

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