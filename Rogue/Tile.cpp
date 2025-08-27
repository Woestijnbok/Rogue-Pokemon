#include "Tile.h"

Tile::Tile() :
	Type{ Terrain::Dirt }
{

}

Tile::Tile(Terrain terrain) :
	Type{ terrain }
{

}