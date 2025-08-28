#include "TileManagerComponent.h"

// Core
#include "Engine.h"
#include "GameObject.h"
#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"
#include "ResourceManager.h"

// Components
#include "TrainerComponent.h"
#include "PokemonComponent.h"

// Other
#include "Enums.h"

using namespace Minigin;

TileManagerComponent::TileManagerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_TileSize{ Engine::GetWindowSize().y / m_Rows },
	m_TileRenderScale{ 1.0f },
	m_TileChanceGrass{ 10.0f },
	m_TileChancePokemon{ 5.0f },
	m_TileChanceItem{ 1.0f },
	m_StartTile{ 5, 5 },
	m_Tiles{},
	m_TileDirtTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Dirt.png") },
	m_TileGrassTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Grass.png") },
	m_TileItemTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Item.png") },
	m_RandomDevice{},
	m_RandomEngine{ m_RandomDevice() },
	m_ChanceDistribution{ 0.0f, 100.0f },
	m_OnPokemonEncounter{}
{
	m_TileRenderScale = static_cast<float>(m_TileSize) / static_cast<float>(m_TileDirtTexture->GetSize().x);
	RandomizeTiles();
}

void TileManagerComponent::Render() const
{
	for (size_t row{}; row < m_Rows; ++row)
	{
		for (size_t collumn{}; collumn < m_Collumns; ++collumn)
		{
			RenderTile(row, collumn);
		}
	}
}

glm::ivec2 TileManagerComponent::GetStartTile() const
{
	return m_StartTile;
}

size_t TileManagerComponent::GetTileSize() const
{
	return m_TileSize;
}

bool TileManagerComponent::CanMove(const glm::ivec2& position, Direction direction) const
{
	bool canMove{ false };
	glm::ivec2 currentTileIndices{ GetTileIndices(position) };

	switch (direction)
	{
	case Direction::Up:
		canMove = currentTileIndices.x + 1 < m_Rows;
		break;
	case Direction::Right:
		canMove = currentTileIndices.y + 1 < m_Collumns;
		break;
	case Direction::Down:
		canMove = currentTileIndices.x > 0;
		break;
	case Direction::Left:
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

	if (GetTile(tileIndices.x, tileIndices.y).Type == Terrain::Pokemon)
	{
		m_OnPokemonEncounter.Notify(trainer);
	}
}

void TileManagerComponent::RenderTile(const size_t row, const size_t collumn) const
{
	const Tile& tile{ m_Tiles.at((row * m_Collumns) + collumn) };
	const Transform transform{ glm::ivec2{ (collumn * m_TileSize) + (m_TileSize / 2), (row * m_TileSize) + (m_TileSize / 2) }, 0, glm::vec2{ m_TileRenderScale }};

	switch (tile.Type)
	{
		case Terrain::Dirt:
			Renderer::Instance()->RenderTexture(*m_TileDirtTexture, transform);
			break;
		case Terrain::Grass:
			Renderer::Instance()->RenderTexture(*m_TileGrassTexture, transform);
			break;
		case Terrain::Item:
			Renderer::Instance()->RenderTexture(*m_TileItemTexture, transform);
			break;
		case Terrain::Pokemon:
			Renderer::Instance()->RenderTexture(*m_TileGrassTexture, transform);
#ifdef _DEBUG
			Renderer::Instance()->RenderDebugBox(glm::ivec2{ collumn * m_TileSize, row * m_TileSize }, glm::ivec2{ (collumn * m_TileSize) + m_TileSize, (row * m_TileSize) + m_TileSize }, Color{ 255, 0, 0, 100 }, true);
#endif // _DEBUG
			break;
		default:
			assert(false);
	}
}

void TileManagerComponent::RandomizeTiles()
{
	for (size_t row{}; row < m_Rows; ++row)
	{
		for (size_t collumn{}; collumn < m_Collumns; ++collumn)
		{
			const float randomValue{ m_ChanceDistribution(m_RandomEngine) };

			if (randomValue < m_TileChanceGrass)
			{
				m_Tiles.at((row * m_Collumns) + collumn).Type = Terrain::Grass;
			}
			else if (randomValue < m_TileChanceGrass + m_TileChancePokemon)
			{
				m_Tiles.at((row * m_Collumns) + collumn).Type = Terrain::Pokemon;
			}
			else if (randomValue < m_TileChanceGrass + m_TileChancePokemon + m_TileChanceItem)
			{
				m_Tiles.at((row * m_Collumns) + collumn).Type = Terrain::Item;
			}
		}
	}

	m_Tiles.at((m_StartTile.x * m_Collumns) + m_StartTile.y).Type = Terrain::Dirt;
}