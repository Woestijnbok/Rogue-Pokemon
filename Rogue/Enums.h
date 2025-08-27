#pragma once

// Libraries
#include <cstdint>

enum class Direction : uint8_t
{
	Up,
	Right,
	Down,
	Left
};

enum class Terrain : uint8_t
{
	Dirt,
	Grass,
	Item,
	Pokemon
};