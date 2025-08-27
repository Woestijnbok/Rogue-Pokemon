#pragma once

// Libraries
#include <cstdint>
#include <string>

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
	explicit Move(const std::string& name, uint8_t power);
	~Move() = default;

	Move(const Move& other) = default;
	Move(Move&& other) noexcept = default;
	Move& operator=(const Move& other) = default;
	Move& operator=(Move&& other) noexcept = default;
	
	// Move name
	const std::string Name;
	// Move power
	const uint8_t Power;
};