#pragma once

// Libraries
#include <cstdint>
#include <filesystem>

namespace Minigin
{
	class SpriteComponent;
	class GameObject;
	class Scene;
}

struct PODPokemon;
class TileManagerComponent;
class MovementComponent;
class BattleManagerComponent;
class TrainerComponent;
class MovementComponent;
class TileManagerComponent;

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

/*
* Focuses the battle scene and disables the world scene.
* 
* @param world: The world scene to disable.
* @param battle: The battle scene to enable.
*/
void FocusBattle(Minigin::Scene* world, Minigin::Scene* battle);

/*
* Focuses the world scene and disables the battle scene.
* 
* @param world: The world scene to enable.
* @param battle: The battle scene to disable.
*/
void FocusWorld(Minigin::Scene* world, Minigin::Scene* battle);

/*
* Connects mutliple logic to battle events from the battle manager component.
* 
* @param battleManager: The battle manager component to connect events to.
* @param world: The world scene to connect events to.
* @param battle: The battle scene to connect events to.
* @param tileManager: The tile manager component to connect events to.
* @param trainer: The trainer component to connect events to.
* @param movementComponent: The movement component to connect events to.
* @param spriteComponent: The sprite component to connect events to.
*/
void ConnectBattleEvents(BattleManagerComponent* battleManager, Minigin::Scene* world, Minigin::Scene* battle, TileManagerComponent* tileManager, TrainerComponent* trainer, MovementComponent* movementComponent, Minigin::SpriteComponent* spriteComponent);

/*
* Sets up all audio for the game, links to certain events.
* 
* @param battleManager: The battle manager component to connect events to.
*/
void SetupAudio(BattleManagerComponent* battleManager, TileManagerComponent* tileManager);