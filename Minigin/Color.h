#pragma once
#include <cstdint>

struct Color final
{
public:
	explicit Color();
	explicit Color(uint8_t red, uint8_t green, uint8_t blue);
	explicit Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	virtual ~Color() = default;

	Color(const Color& other) = default;
	Color(Color&& other) noexcept = default;
	Color& operator=(const Color& other) = default;
	Color& operator=(Color&& other) noexcept = default;

	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};