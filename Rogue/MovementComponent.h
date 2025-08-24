#pragma once

#include  <optional>
#include <vec2.hpp>

#include "Component.h"

class TileManagerComponent;

namespace Minigin
{
	class SpriteComponent;
}

class MovementComponent final : public Minigin::Component
{
public:
	enum class Direction
	{
		Up,
		Right,
		Down,
		Left
	};

	explicit MovementComponent(Minigin::GameObject* owner, TileManagerComponent* tileManager);
	virtual ~MovementComponent() = default;

	MovementComponent(const MovementComponent& other) = delete;
	MovementComponent(MovementComponent&& other) noexcept = delete;
	MovementComponent& operator=(const MovementComponent& other) = delete;
	MovementComponent& operator=(MovementComponent&& other) noexcept = delete;

	virtual void Update() override;

	void Move(Direction direction);
	Direction GetDirection() const;
	void SetSpeed(float speed);

private:
	void SetDirection(Direction direction);
	void TeleportToStartTile();
	void CompleteMovement(glm::ivec2& newPosition);
	void AddMovementSrites();

	Direction m_Direction;
	bool m_Moving;
	std::optional<glm::ivec2> m_TargetPosition;
	TileManagerComponent* m_TileManagerComponent;
	float m_Speed;
	Minigin::SpriteComponent* m_SpriteComponent;
};