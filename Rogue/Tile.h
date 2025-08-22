#pragma once

#include <cstdint>

class Tile final
{
public:

	enum class Terrain : uint8_t
	{
		Dirt,
		Grass
	};

	Tile();
	explicit Tile(Terrain terrain);
	~Tile() = default;

	Tile(const Tile& other) = default;
	Tile(Tile&& other) noexcept = default;
	Tile& operator=(const Tile& other) = default;
	Tile& operator=(Tile&& other) noexcept = default;

	Terrain GetTerrain() const;
	void SetTerrain(Terrain terrain);

private:
	Terrain m_Terrain;
};