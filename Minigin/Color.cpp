#include "Color.h"
#include <limits>
#include <algorithm>

const Color Color::Black{ 0, 0, 0 };
const Color Color::White{ 255, 255, 255 };
const Color Color::Red{ 255, 0, 0 };
const Color Color::Green{ 0, 255, 0 };
const Color Color::Blue{ 0, 0, 255 };

Color::Color() :
	r{ 0 },
	g{ 0 },
	b{ 0 },
	a{ 255 }
{

}

Color::Color(uint8_t red, uint8_t green, uint8_t blue) :
	r{ std::min(std::numeric_limits<uint8_t>::max(), red) },
	g{ std::min(std::numeric_limits<uint8_t>::max(), green) },
	b{ std::min(std::numeric_limits<uint8_t>::max(), blue) },
	a{ 255 }
{

}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
	r{ std::min(std::numeric_limits<uint8_t>::max(), red) },
	g{ std::min(std::numeric_limits<uint8_t>::max(), green) },
	b{ std::min(std::numeric_limits<uint8_t>::max(), blue) },
	a{ std::min(std::numeric_limits<uint8_t>::max(), alpha) }
{

}