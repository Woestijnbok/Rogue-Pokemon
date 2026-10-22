#pragma once

#include <cstdint>

enum class RenderOrder : uint8_t
{
	Background,
	World,
	Player,
	Foreground,
	UI
};