#pragma once
#include <cstdint>
#include <algorithm>

struct Color final
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	explicit constexpr Color() :
		r{ 0 },
		g{ 0 },
		b{ 0 },
		a{ 255 }
	{

	}

	explicit constexpr Color(uint8_t red, uint8_t green, uint8_t blue) :
		r{ std::min(static_cast<uint8_t>(255), red) },
		g{ std::min(static_cast<uint8_t>(255), green) },
		b{ std::min(static_cast<uint8_t>(255), blue) },
		a{ 255 }
	{

	}

	explicit constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
		r{ std::min(std::numeric_limits<uint8_t>::max(), red) },
		g{ std::min(std::numeric_limits<uint8_t>::max(), green) },
		b{ std::min(std::numeric_limits<uint8_t>::max(), blue) },
		a{ std::min(std::numeric_limits<uint8_t>::max(), alpha) }
	{

	}

	~Color() = default;

	Color(const Color& other) = default;
	Color(Color&& other) noexcept = default;
	Color& operator=(const Color& other) = default;
	Color& operator=(Color&& other) noexcept = default;

	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};