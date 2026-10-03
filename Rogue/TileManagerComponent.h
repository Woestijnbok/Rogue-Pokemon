#pragma once

// Libraries
#include <array>
#include <memory>
#include <random>
#include <vec2.hpp>

// Core
#include "Component.h"
#include "Subject.h"

// Other
#include "Enums.h"
#include "Tile.h"

namespace Minigin
{
	class Texture;
}

class TrainerComponent;

/*
* Manager component used to genrate and control all tiles.
*/
class TileManagerComponent final : public Minigin::Component
{
public:
	/*
	* Construct tile manager component.
	* 
	* @param owner: The game object that will own this manager.
	*/
	explicit TileManagerComponent(Minigin::GameObject* owner);
	virtual ~TileManagerComponent() = default;

	TileManagerComponent(const TileManagerComponent& other) = delete;
	TileManagerComponent(TileManagerComponent&& other) noexcept = delete;
	TileManagerComponent& operator=(const TileManagerComponent& other) = delete;
	TileManagerComponent& operator=(TileManagerComponent&& other) noexcept = delete;

	/*
	* Renders all tiles.
	*/
	virtual void Render() const override;

	/*
	* Gets the first open tile, aka dirt tile.
	* 
	* @return Indices to the start tile.
	*/
	glm::ivec2 GetStartTile() const;
	/*
	* Gets the the tile size in pixels.
	* 
	* @return The tile size.
	*/
	size_t GetTileSize() const;
	/*
	* Checks if an game object can move from a certain world position to a set direction.
	* 
	* @param position: World position.
	* @param direction: The move direction.
	* 
	* @return Wether or not a move towards the given direction can be done, true = yes.
	*/
	bool CanMove(const glm::ivec2& position, Direction direction) const;
	/*
	* Gets render scale applied to all tiles.
	* 
	* @return The render scale for all tiles.
	*/
	float GetRenderScale() const;
	/*
	* Get tile indices based on a given world position.
	* 
	* @param position: The world position.
	* 
	* @return The tile indices based on the world position.
	*/
	glm::ivec2 GetTileIndices(const glm::ivec2& position) const;
	/*
	* Gets a tile based on the given tile indices.
	* 
	* @param row: The row index.
	* @param collumn: The collumn index.
	* 
	* @return The tile that resides in the given tile indices.
	*/
	const Tile& GetTile(int row, int collumn) const;
	/*
	* Checks wether or not the trainer has encountered a pokemon.
	* 
	* @param trainer: The trainer to check for.
	*/
	void CheckTile(TrainerComponent* trainer);
	/*
	* Gets the pokemon encounter event.
	* 
	* @return The on pokemon encounter event.
	*/
	Minigin::Subject<TrainerComponent*>& OnPokemonEncounter();
	/*
	* Gets the item encounter event.
	*
	* @return The on item encounter event.
	*/
	Minigin::Subject<>& OnItemEncounter();
	/*
	* Resets the tile manager to a new starting tile set.
	*/
	void Reset();
	/*
	* Increases the amount of defeated pokemon in this tile set by 1.
	* Goes to the next level if the amount of defeated pokemon is enough to complete the level.
	*/
	void DefeatedPokemon();
	/*
	* Gets the level defeated event.
	*
	* @return The on level defeated event.
	*/
	Minigin::Subject<>& OnLevelDefeated();

private:
	/*
	* Renders the tile with the given indices.
	* 
	* @param row: The row index of the tile
	* @param collumn: The collumn index of the tile.
	*/
	void RenderTile(const size_t row, const size_t collumn) const;
	/*
	* Randomizes all tiles to have random terrains.
	*/
	void RandomizeTiles();
	/*
	* Sets up the new tileset / level.
	*/
	void SetupNextLevel();

	// Amount of rows
	static constexpr int m_Rows{ 10 };
	// Amount of collumns
	static constexpr int m_Collumns{ 20 };
	// Tile size in pixels
	const int m_TileSize;
	// Render scale for all tile textures
	float m_TileRenderScale;
	// Start Chance to generate a tile with grass at first tile set / level
	const float m_StartTileChanceGrass;
	// Start Chance to generate a tile with a pokemon at first tile set / level
	const float m_StartTileChancePokemon;
	// Start Chance to generate a tile with an item at first tile set / level
	const float m_StartTileChanceItem;
	// Multiplier to change the chance to generate a tile with grass for each new tile set / level in %
	const float m_MultiplierTileChanceGrass;
	// Multiplier to change the chance to generate a tile with a pokemon for each new tile set / level in %
	const float m_MultiplierTileChancePokemon;
	// Multiplier to change the chance to generate a tile with an item for each new tile set / level in %
	const float m_MultiplierTileChanceItem;
	// Chance to generate a tile with grass
	float m_TileChanceGrass;
	// Chance to generate a tile with a pokemon
	float m_TileChancePokemon;
	// Chance to generate a tile with an item
	float m_TileChanceItem;
	// Start tile indices, this tile will always be dirt
	const glm::ivec2 m_StartTile;
	// Container containing all tiles
	std::array<Tile, m_Collumns * m_Rows> m_Tiles;
	// Dirt tile texture
	const std::unique_ptr<Minigin::Texture> m_TileDirtTexture;
	// Grass tile textures=
	const std::unique_ptr<Minigin::Texture> m_TileGrassTexture;
	// Item tile texture
	const std::unique_ptr<Minigin::Texture> m_TileItemTexture;
	// Random device used to generate seeds
	std::random_device m_RandomDevice;
	// Random engine generate random numbers
	std::mt19937 m_RandomEngine;
	// Chance distribution
	std::uniform_real_distribution<float> m_ChanceDistribution;
	// On pokemon encounter event
	Minigin::Subject<TrainerComponent*> m_OnPokemonEncounter;
	// On item pickup event
	Minigin::Subject<> m_OnItemPickup;
	// Number of spawned pokemon this tileset in the begining
	uint8_t m_SpawnedPokemon;
	// Number of defeated pokemon this tileset
	uint8_t m_DefeatedPokemon;
	// Ration in % of pokemon that needs to be defeated to complete the level
	const float m_LevelDefeatRatio;
	// On level / tileset defeated event
	Minigin::Subject<> m_OnLevelDefeated;
};