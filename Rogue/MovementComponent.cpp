#include "MovementComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "TileManagerComponent.h"

using namespace Minigin;

MovementComponent::MovementComponent(Minigin::GameObject* owner, TileManagerComponent* tileManager) :
	Component{ owner },
	m_Direction{ Direction::Down },
	m_TargetPosition{},
	m_TileManagerComponent{ tileManager },
	m_Speed{ 500.0f }
{
	TeleportToStartTile();
}

void MovementComponent::Update()
{
	if (m_Moving)
	{
		assert(m_TargetPosition.has_value());

		const glm::ivec2 oldPosition{ GetOwner()->GetLocalTransform().GetPosition() };
		glm::ivec2 newPosition{ oldPosition };

		switch (m_Direction)
		{
		case Direction::Up:
			newPosition.y += static_cast<int>(m_Speed * Minigin::TimeManager::Instance()->GetDeltaTime().count());
			if (newPosition.y > m_TargetPosition.value().y)
			{
				CompleteMovement(newPosition);
			}
			break;
		case Direction::Right:
			newPosition.x += static_cast<int>(m_Speed * Minigin::TimeManager::Instance()->GetDeltaTime().count());
			if (newPosition.x > m_TargetPosition.value().x)
			{
				CompleteMovement(newPosition);
			}
			break;
		case Direction::Down:
			newPosition.y -= static_cast<int>(m_Speed * Minigin::TimeManager::Instance()->GetDeltaTime().count());
			if (newPosition.y < m_TargetPosition.value().y)
			{
				CompleteMovement(newPosition);
			}
			break;
		case Direction::Left:
			newPosition.x -= static_cast<int>(m_Speed * Minigin::TimeManager::Instance()->GetDeltaTime().count());
			if (newPosition.x < m_TargetPosition.value().x)
			{
				CompleteMovement(newPosition);
			}
			break;
		default:
			throw std::exception{ "MovementComponent::Update() - Invalid direction." };
		}

		GetOwner()->SetLocalPosition(newPosition);
	}
}

void MovementComponent::Move(Direction direction)
{
	if (!m_Moving)
	{
		const glm::ivec2 currentPosition{ GetOwner()->GetLocalTransform().GetPosition() };

		if(m_TileManagerComponent->CanMove(currentPosition, direction))
		{
			switch (direction)
			{
			case Direction::Up:
				m_TargetPosition.emplace(currentPosition.x, currentPosition.y + static_cast<int>(m_TileManagerComponent->GetTileSize()));
				break;
			case Direction::Right:
				m_TargetPosition.emplace(currentPosition.x + static_cast<int>(m_TileManagerComponent->GetTileSize()), currentPosition.y);
				break;
			case Direction::Down:
				m_TargetPosition.emplace(currentPosition.x, currentPosition.y - static_cast<int>(m_TileManagerComponent->GetTileSize()));
				break;
			case Direction::Left:
				m_TargetPosition.emplace(currentPosition.x - static_cast<int>(m_TileManagerComponent->GetTileSize()), currentPosition.y);
				break;
			default:
				throw std::exception{ "MovementComponent::Update() - Invalid direction." };
			}

			m_Moving = true;
			SetDirection(direction);
		}
	}
}

MovementComponent::Direction MovementComponent::GetDirection() const
{
	return m_Direction;
}

void MovementComponent::SetSpeed(float speed)
{
	m_Speed = speed;
}

void MovementComponent::SetDirection(Direction direction)
{
	m_Direction = direction;
}

void MovementComponent::TeleportToStartTile()
{
	const glm::ivec2 startTile{ m_TileManagerComponent->GetStartTile() };
	const size_t tileSize{ m_TileManagerComponent->GetTileSize() };
	const glm::ivec2 startPosition{ glm::ivec2{ (startTile.y * tileSize) + (tileSize / 2), (startTile.x * tileSize) + (tileSize / 2) } };
	GetOwner()->SetLocalPosition(startPosition);
}

void MovementComponent::CompleteMovement(glm::ivec2& newPosition)
{
	assert(m_TargetPosition.has_value() and m_Moving);

	newPosition = m_TargetPosition.value();
	m_Moving = false;
	m_TargetPosition.reset();
}