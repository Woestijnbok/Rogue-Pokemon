#include <vec2.hpp>

#include "TileManagerComponent.h"
#include "Renderer.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Engine.h"

#define GRASS_TILE_CHANCE 7

using namespace Minigin;

TileManagerComponent::TileManagerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_Tiles{},
	m_TileDirtTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Dirt.png") },
	m_TileGrassTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Grass.png") },
	m_TileSize{ Engine::GetWindowSize().x / m_Rows },
	m_TileRenderScale{ static_cast<float>(m_TileSize) / static_cast<float>(m_TileDirtTexture->GetSize().x) },
	m_RandomDevice{},
	m_RandomEngine{ m_RandomDevice() },
	m_RandomDistribution{ 0, 100 }
{
	RandomizeTiles();
}

void TileManagerComponent::Render() const
{
	for (size_t row{}; row < m_Rows; ++row)
	{
		for (size_t collumn{}; collumn < m_Collumns; ++collumn)
		{
			const size_t index{ (row * m_Collumns) + collumn };

			RenderTile(m_Tiles.at(index), row, collumn);
		}
	}
}

void TileManagerComponent::RenderTile(const Tile& tile, const size_t row, const size_t collumn) const
{
	const Transform transform{ glm::ivec2{ (row * m_TileSize) + (m_TileSize / 2), (collumn * m_TileSize) + (m_TileSize / 2)}, 0, glm::vec2{ m_TileRenderScale }};

	switch (tile.GetTerrain())
	{
		case Tile::Terrain::Dirt:
			Renderer::Instance()->RenderTexture(*m_TileDirtTexture, transform);
			break;
		case Tile::Terrain::Grass:
			Renderer::Instance()->RenderTexture(*m_TileGrassTexture, transform);
			break;
		default:
			throw std::runtime_error("TileManagerComponent::RenderTile() - Unsupported terrain type");
	}
}

void TileManagerComponent::RandomizeTiles()
{
	for (size_t row{}; row < m_Rows; ++row)
	{
		for (size_t collumn{}; collumn < m_Collumns; ++collumn)
		{
			if (m_RandomDistribution(m_RandomEngine) < GRASS_TILE_CHANCE)
			{
				m_Tiles.at((row * m_Collumns) + collumn).SetTerrain(Tile::Terrain::Grass);
			}
		}
	}
}