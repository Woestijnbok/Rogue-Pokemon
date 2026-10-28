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
	Pokemon,
	LadderLow,
	LadderHigh
};

enum class ItemType : uint8_t
{
	FullRestore,
	RareCandy,
	Count
};

enum class MoveType : uint8_t
{
	Physical,
	Special
};

enum class BattleState : uint8_t
{
	SelectMove,
	PerformPlayerMove,
	PerformEnemyMove,
	MissedMove,
	CritMove,
	Effectiveness,
	None
};