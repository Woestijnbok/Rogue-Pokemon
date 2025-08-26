#pragma once

#include <array>
#include <string>

#include "Component.h"

struct PODPokemon;

struct Move
{
	explicit Move(const std::string& name, uint8_t power);
	~Move() = default;

	Move(const Move& other) = default;
	Move(Move&& other) noexcept = default;
	Move& operator=(const Move& other) = default;
	Move& operator=(Move&& other) noexcept = default;

	const std::string Name;
	const uint8_t Power;
};

class PokemonComponent final : public Minigin::Component
{
public:
	explicit PokemonComponent(Minigin::GameObject* owner, const PODPokemon& pokemon);
	virtual ~PokemonComponent() = default;

	PokemonComponent(const PokemonComponent& other) = delete;
	PokemonComponent(PokemonComponent&& other) noexcept = delete;
	PokemonComponent& operator=(const PokemonComponent& other) = delete;
	PokemonComponent& operator=(PokemonComponent&& other) noexcept = delete;

	const std::string& GetName() const;

private:
	const std::array<Move, 4> m_Moves;
	const std::string m_Name;
};