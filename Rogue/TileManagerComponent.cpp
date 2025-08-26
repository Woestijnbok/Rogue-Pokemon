#include <vec2.hpp>
#include <algorithm>

#include "TileManagerComponent.h"
#include "GameObject.h"
#include "TrainerComponent.h"
#include "Renderer.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Engine.h"
#include "PokemonComponent.h"

using namespace Minigin;

TileManagerComponent::TileManagerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_Tiles{},
	m_TileDirtTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Dirt.png") },
	m_TileGrassTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Grass.png") },
	m_TileItemTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Item.png") },
	m_TileSize{ Engine::GetWindowSize().y / m_Rows },
	m_TileRenderScale{ static_cast<float>(m_TileSize) / static_cast<float>(m_TileDirtTexture->GetSize().x) },
	m_RandomDevice{},
	m_RandomEngine{ m_RandomDevice() },
	m_RandomDistribution{ 0, 100 },
	m_OnPokemonEncounter{}
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

glm::ivec2 TileManagerComponent::GetStartTile() const
{
	glm::ivec2 tile{ -1, -1 };
	size_t index{ 0 };

	do
	{
		if (m_Tiles.at(index).GetTerrain() == Tile::Terrain::Dirt)
		{
			tile.x = int(index / m_Collumns);
			tile.y = int(index % m_Collumns);
			return tile;
		}

		++index;
	} 
	while (index < m_Tiles.size() and tile.x == -1);

	return tile;
}

size_t TileManagerComponent::GetTileSize() const
{
	return m_TileSize;
}

bool TileManagerComponent::CanMove(const glm::ivec2& position, MovementComponent::Direction direction) const
{
	bool canMove{ false };
	glm::ivec2 currentTileIndices{ GetTileIndices(position) };

	switch (direction)
	{
	case MovementComponent::Direction::Up:
		canMove = currentTileIndices.x + 1 < m_Rows;
		break;
	case MovementComponent::Direction::Right:
		canMove = currentTileIndices.y + 1 < m_Collumns;
		break;
	case MovementComponent::Direction::Down:
		canMove = currentTileIndices.x > 0;
		break;
	case MovementComponent::Direction::Left:
		canMove = currentTileIndices.y > 0;
		break;
	}

	return canMove;
}

float TileManagerComponent::GetRenderScale() const
{
	return m_TileRenderScale;
}

glm::ivec2 TileManagerComponent::GetTileIndices(const glm::ivec2& position) const
{
	return glm::ivec2{ position.y / static_cast<int>(m_TileSize), position.x / static_cast<int>(m_TileSize) };
}

Minigin::Subject<TrainerComponent*>& TileManagerComponent::OnPokemonEncounter()
{
	return m_OnPokemonEncounter;
}

const Tile& TileManagerComponent::GetTile(int row, int collumn) const
{
	return m_Tiles.at((row * m_Collumns) + collumn);
}

void TileManagerComponent::CheckForBattle(TrainerComponent* trainer)
{
	const glm::ivec2 tileIndices{ GetTileIndices(trainer->GetOwner()->GetWorldTransform().GetPosition()) };

	if (GetTile(tileIndices.x, tileIndices.y).GetTerrain() == Tile::Terrain::Pokemon)
	{
		m_OnPokemonEncounter.Notify(trainer);
	}
}

void TileManagerComponent::RenderTile(const Tile& tile, const size_t row, const size_t collumn) const
{
	const Transform transform{ glm::ivec2{ (collumn * m_TileSize) + (m_TileSize / 2), (row * m_TileSize) + (m_TileSize / 2) }, 0, glm::vec2{ m_TileRenderScale }};

	switch (tile.GetTerrain())
	{
		case Tile::Terrain::Dirt:
			Renderer::Instance()->RenderTexture(*m_TileDirtTexture, transform);
			break;
		case Tile::Terrain::Grass:
			Renderer::Instance()->RenderTexture(*m_TileGrassTexture, transform);
			break;
		case Tile::Terrain::Item:
			Renderer::Instance()->RenderTexture(*m_TileItemTexture, transform);
			break;
		case Tile::Terrain::Pokemon:
			Renderer::Instance()->RenderTexture(*m_TileGrassTexture, transform);
#ifdef _DEBUG
			Renderer::Instance()->RenderDebugBox(glm::ivec2{ collumn * m_TileSize, row * m_TileSize }, glm::ivec2{ (collumn * m_TileSize) + m_TileSize, (row * m_TileSize) + m_TileSize }, Color{ 255, 0, 0, 100 }, true);
#endif // _DEBUG
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
			const int randomValue{ m_RandomDistribution(m_RandomEngine) };

			if (randomValue < m_TileChanceGrass)
			{
				m_Tiles.at((row * m_Collumns) + collumn).SetTerrain(Tile::Terrain::Grass);
			}
			else if (randomValue < m_TileChanceGrass + m_TileChancePokemon)
			{
				m_Tiles.at((row * m_Collumns) + collumn).SetTerrain(Tile::Terrain::Pokemon);
			}
			else if (randomValue < m_TileChanceGrass + m_TileChancePokemon + m_TileChanceItem)
			{
				m_Tiles.at((row * m_Collumns) + collumn).SetTerrain(Tile::Terrain::Item);
			}
		}
	}
}