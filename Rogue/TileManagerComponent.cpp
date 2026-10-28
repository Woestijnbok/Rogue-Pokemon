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
	m_StartTileChanceGrass{ 10.0f },
	m_StartTileChancePokemon{ 2.0f },
	m_StartTileChanceItem{ 2.0f },
	m_MultiplierTileChanceGrass{ 150.0f },
	m_MultiplierTileChancePokemon{ 120.0f },
	m_MultiplierTileChanceItem{ 120.0f },
	m_TileChanceGrass{ m_StartTileChanceGrass },
	m_TileChancePokemon{ m_StartTileChancePokemon },
	m_TileChanceItem{ m_StartTileChanceItem },
	m_ItemChances{ 50.0f, 50.0f },
	m_StartTile{ 5, 5 },
	m_Tiles{},
	m_TileDirtTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Dirt.png") },
	m_TileGrassTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Grass.png") },
	m_TileItemTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Item.png") },
	m_LowerLadderTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Ladder Low.jpg") },
	m_UpperLadderTexture{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Ladder High.jpg") },
	m_RandomDevice{},
	m_RandomEngine{ m_RandomDevice() },
	m_ChanceDistribution{ 0.0f, 100.0f },
	m_OnPokemonEncounter{},
	m_TileChanceDistribution{ 0, (m_Rows * m_Collumns) - 1 },
	m_PossibleLadderTiles{ 0, (m_Rows * m_Collumns) - 1 - m_Collumns },
	m_OnItemPickup{},
	m_OnLadderUse{},
	m_SpawnedPokemon{},
	m_DefeatedPokemon{},
	m_LevelDefeatRatio{ 50.0f },
	m_OnLevelDefeated{},
	m_LadderHasSpawned{ false },
	m_Player{ nullptr }
{
	SetRenderOrder(RenderOrder::World);
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

Minigin::Subject<const ItemType>& TileManagerComponent::OnItemEncounter()
{
	return m_OnItemPickup;
}

const Tile& TileManagerComponent::GetTile(int row, int collumn) const
{
	return m_Tiles.at((row * m_Collumns) + collumn);
}

void TileManagerComponent::CheckTile()
{
	const glm::ivec2 tileIndices{ GetTileIndices(m_Player->GetOwner()->GetWorldTransform().GetPosition())};

	switch (GetTile(tileIndices.x, tileIndices.y).Type)
	{
	case Terrain::Pokemon:
		m_Tiles.at((tileIndices.x * m_Collumns) + tileIndices.y).Type = Terrain::Grass;
		m_OnPokemonEncounter.Notify(m_Player);
		break;
	case Terrain::Item:
		{
			// Remove the item from the tileset and change it to a dirt tile
			m_Tiles.at((tileIndices.x * m_Collumns) + tileIndices.y).Type = Terrain::Dirt;

			// Generate a random item
			ItemType randomItem{ ItemType::FullRestore };
			const float randomValue{ m_ChanceDistribution(m_RandomEngine) };
			if (randomValue < m_ItemChances.at(static_cast<size_t>(ItemType::FullRestore)))
			{
				randomItem = ItemType::FullRestore;
			}
			else
			{
				randomItem = ItemType::RareCandy;
			}

			// Notify the pickup of the random item
			m_OnItemPickup.Notify(randomItem);
		}
		break;
	case Terrain::LadderLow:
		SetupNextLevel();
		m_OnLadderUse.Notify();
		break;
	default:
		break;
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
			Renderer::Instance()->RenderBox(glm::ivec2{ collumn * m_TileSize, row * m_TileSize }, glm::ivec2{ (collumn * m_TileSize) + m_TileSize, (row * m_TileSize) + m_TileSize }, Color{ 255, 0, 0, 100 }, true);
#endif // _DEBUG
			break;
		case Terrain::LadderLow:
			Renderer::Instance()->RenderTexture(*m_LowerLadderTexture, transform);
			break;
		case Terrain::LadderHigh:
			Renderer::Instance()->RenderTexture(*m_UpperLadderTexture, transform);
			break;
		default:
			assert(false);
	}
}

void TileManagerComponent::RandomizeTiles()
{
	m_SpawnedPokemon = 0;

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
				++m_SpawnedPokemon;
			}
			else if (randomValue < m_TileChanceGrass + m_TileChancePokemon + m_TileChanceItem)
			{
				m_Tiles.at((row * m_Collumns) + collumn).Type = Terrain::Item;
			}
			else
			{
				m_Tiles.at((row * m_Collumns) + collumn).Type = Terrain::Dirt;
			}
		}
	}

	// If start tile was a pokemon decrease pokemon count
	size_t startTileIndex{ static_cast<size_t>((m_StartTile.x * m_Collumns) + m_StartTile.y) };
	if (m_Tiles.at(startTileIndex).Type == Terrain::Pokemon)
	{
		--m_SpawnedPokemon;
	}
	m_Tiles.at((m_StartTile.x * m_Collumns) + m_StartTile.y).Type = Terrain::Dirt;
	
	// The unique case that a tileset was generated with no pokemon
	if (m_SpawnedPokemon == 0)
	{
		size_t randomTileIndex{};
		
		do
		{
			randomTileIndex = m_TileChanceDistribution(m_RandomEngine);
		} 
		while (randomTileIndex == startTileIndex);

		m_Tiles.at(randomTileIndex).Type = Terrain::Pokemon;
		m_SpawnedPokemon++;
	}
}

void TileManagerComponent::SetupNextLevel()
{
	m_LadderHasSpawned = false;
	m_DefeatedPokemon = 0;

	m_TileChanceGrass *= (m_MultiplierTileChanceGrass / 100.0f);
	m_TileChancePokemon *= (m_MultiplierTileChancePokemon / 100.0f);
	m_TileChanceItem *= (m_MultiplierTileChanceItem / 100.0f);

	m_TileChancePokemon = std::min(m_TileChancePokemon, 100.0f);
	m_TileChanceItem = std::min(m_TileChanceItem, 100.0f - m_TileChancePokemon);
	m_TileChanceGrass = std::min(m_TileChanceGrass, 100.0f - m_TileChancePokemon - m_TileChanceItem);

	RandomizeTiles();
}

void TileManagerComponent::SpawnLadder()
{
	const glm::ivec2 playterTileIndices{ GetTileIndices(m_Player->GetOwner()->GetWorldTransform().GetPosition()) };
	const size_t playerTileIndex{ static_cast<size_t>(playterTileIndices.x * m_Collumns + playterTileIndices.y) };
	size_t randomLowerLadderTileIndex{ static_cast<size_t>(-1) };

	do
	{
		randomLowerLadderTileIndex = m_PossibleLadderTiles(m_RandomEngine);
	} 
	while (randomLowerLadderTileIndex == playerTileIndex);
	
	if (m_Tiles.at(randomLowerLadderTileIndex).Type == Terrain::Pokemon)
	{
		--m_SpawnedPokemon;
	}
	if (m_Tiles.at(randomLowerLadderTileIndex + m_Collumns).Type == Terrain::Pokemon)
	{
		--m_SpawnedPokemon;
	}

	m_Tiles.at(randomLowerLadderTileIndex).Type = Terrain::LadderLow;
	m_Tiles.at(randomLowerLadderTileIndex + m_Collumns).Type = Terrain::LadderHigh;
	m_LadderHasSpawned = true;
}

Minigin::Subject<>& TileManagerComponent::OnLadderUse()
{
	return m_OnLadderUse;
}

void TileManagerComponent::Reset()
{
	m_SpawnedPokemon = 0;
	m_DefeatedPokemon = 0;
	m_TileChanceGrass = m_StartTileChanceGrass;
	m_TileChancePokemon = m_StartTileChancePokemon;
	m_TileChanceItem = m_StartTileChanceItem;
	RandomizeTiles();
	m_LadderHasSpawned = false;
}

void TileManagerComponent::DefeatedPokemon()
{
	++m_DefeatedPokemon;
	if (std::round(float(m_DefeatedPokemon) / m_SpawnedPokemon) >= std::round(m_LevelDefeatRatio / 100.0f) and !m_LadderHasSpawned)
	{
		SpawnLadder();
		m_OnLevelDefeated.Notify();
	}
}

Minigin::Subject<>& TileManagerComponent::OnLevelDefeated()
{
	return m_OnLevelDefeated;
}

void TileManagerComponent::CacheTrainer(TrainerComponent* trainer)
{
	m_Player = trainer;
}