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
	void CheckForBattle(TrainerComponent* trainer);
	/*
	* Gets the pokemon encounter event.
	* 
	* @return The on pokemon encounter event.
	*/
	Minigin::Subject<TrainerComponent*>& OnPokemonEncounter();

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

	// Amount of rows
	static constexpr int m_Rows{ 10 };
	// Amount of collumns
	static constexpr int m_Collumns{ 20 };
	// Tile size in pixels
	const int m_TileSize;
	// Render scale for all tile textures
	float m_TileRenderScale;
	// Chance to generate a tile with grass
	const float m_TileChanceGrass;
	// Chance to generate a tile with a pokemon
	const float m_TileChancePokemon;
	// Chance to generate a tile with an item
	const float m_TileChanceItem;
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
};