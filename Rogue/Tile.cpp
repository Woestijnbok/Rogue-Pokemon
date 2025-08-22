#include "Tile.h"

Tile::Tile() :
	m_Terrain{ Terrain::Dirt }
{

}

Tile::Tile(Terrain terrain) :
	m_Terrain{ terrain }
{

}

Tile::Terrain Tile::GetTerrain() const
{
	return m_Terrain;
}

void Tile::SetTerrain(Terrain terrain)
{
	m_Terrain = terrain;
}