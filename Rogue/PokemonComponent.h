#pragma once

#include "Component.h"

class PokemonComponent final : public Minigin::Component
{
public:
	explicit PokemonComponent(Minigin::GameObject* owner);
	virtual ~PokemonComponent() = default;

	PokemonComponent(const PokemonComponent& other) = delete;
	PokemonComponent(PokemonComponent&& other) noexcept = delete;
	PokemonComponent& operator=(const PokemonComponent& other) = delete;
	PokemonComponent& operator=(PokemonComponent&& other) noexcept = delete;

private:

};