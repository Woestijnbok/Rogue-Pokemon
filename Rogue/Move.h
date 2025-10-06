#pragma once

// Libraries
#include <cstdint>
#include <memory>
#include <string>

// Other
#include <Pokedex.hpp>
#include "Enums.h"

namespace Minigin
{
	class Text;
	class Font;
}

/*
* Struct containing all information for a pokemon move / attack.
*/
struct Move
{
	/*
	* Constructs pokemon move struct.
	* 
	* @param name: The move's name.
	* @param power: The move's power.
	*/
	explicit Move(const PODMove& move, Minigin::Font* font);
	~Move() = default;

	Move(const Move& other) = default;
	Move(Move&& other) noexcept = default;
	Move& operator=(const Move& other) = default;
	Move& operator=(Move&& other) noexcept = default;
	
	// Name text
	const std::unique_ptr<Minigin::Text> Name;
	// Move Type
	MoveType Type;
	// Element Type
	ElementType Element;
	// Power
	const uint8_t Power;
	// Accuracy
	const uint8_t Accuracy;
	// Crit chance
	const uint8_t CritChance;

private:
	/*
	* Converts a pokemon's move name to the display name
	*
	* @param name: The move's name straight from the pokedex file.
	*/
	static std::string GetDisplayName(const std::string& name);
};