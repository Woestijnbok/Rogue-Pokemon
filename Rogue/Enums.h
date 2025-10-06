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

enum class MoveType : uint8_t
{
	Physical,
	Special,
	Status
};

enum class ElementType : uint8_t
{
	Normal,
	Fire,
	Water,
	Electric,
	Grass,
	Ice,
	Fighting,
	Poison,
	Ground,
	Flying,
	Psychic,
	Bug,
	Rock,
	Ghost,
	Dragon,
	Dark,
	Steel
};

enum class BattleState : uint8_t
{
	SelectMove,
	PerformPlayerMove,
	PerformEnemyMove,
	EndBattle
};