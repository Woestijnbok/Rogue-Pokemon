#pragma once

#include <array>
#include <memory>
#include <random>

#include "Component.h"
#include "Tile.h"
#include "MovementComponent.h"

namespace Minigin
{
	class Texture;
}

class TileManagerComponent final : public Minigin::Component
{
public:
	explicit TileManagerComponent(Minigin::GameObject* owner);
	virtual ~TileManagerComponent() = default;

	TileManagerComponent(const TileManagerComponent& other) = delete;
	TileManagerComponent(TileManagerComponent&& other) noexcept = delete;
	TileManagerComponent& operator=(const TileManagerComponent& other) = delete;
	TileManagerComponent& operator=(TileManagerComponent&& other) noexcept = delete;

	virtual void Render() const override;
	glm::ivec2 GetStartTile() const;
	size_t GetTileSize() const;
	bool CanMove(const glm::ivec2& position, MovementComponent::Direction direction) const;

private:
	void RenderTile(const Tile& tile, const size_t row, const size_t collumn) const;
	void RandomizeTiles();

	static constexpr size_t m_Rows{ 10 };
	static constexpr size_t m_Collumns{ 20 };
	static constexpr int m_TileChanceGrass{ 10 };
	static constexpr int m_TileChancePokemon{ 5 };
	static constexpr int m_TileChanceItem{ 1 };

	std::array<Tile, m_Collumns * m_Rows> m_Tiles;
	const std::unique_ptr<Minigin::Texture> m_TileDirtTexture;
	const std::unique_ptr<Minigin::Texture> m_TileGrassTexture;
	const std::unique_ptr<Minigin::Texture> m_TileItemTexture;
	const size_t m_TileSize;
	const float m_TileRenderScale;
	std::random_device m_RandomDevice;
	std::mt19937 m_RandomEngine;
	std::uniform_int_distribution<int> m_RandomDistribution;
};