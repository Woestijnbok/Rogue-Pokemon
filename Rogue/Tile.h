#pragma once

// Libraries
#include <cstdint>

// Other
#include "Enums.h"

/*
* Tile struct containing all information a tile holds.
*/
struct Tile final
{
	Tile();
	explicit Tile(Terrain terrain);
	~Tile() = default;

	Tile(const Tile& other) = default;
	Tile(Tile&& other) noexcept = default;
	Tile& operator=(const Tile& other) = default;
	Tile& operator=(Tile&& other) noexcept = default;

	// Terrain type for this tile
	Terrain Type;
};