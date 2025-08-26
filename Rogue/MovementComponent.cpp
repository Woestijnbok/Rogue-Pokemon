#include <cassert>
#include <iostream>
#include <format>

#include "MovementComponent.h"
#include "TrainerComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "TileManagerComponent.h"
#include "SpriteComponent.h"
#include "ResourceManager.h"

using namespace Minigin;

MovementComponent::MovementComponent(Minigin::GameObject* owner, TileManagerComponent* tileManager) :
	Component{ owner },
	m_Direction{ Direction::Down },
	m_TargetPosition{},
	m_TileManagerComponent{ tileManager },
	m_TrainerComponent{ owner->GetComponent<TrainerComponent>() },
	m_Speed{ 200.0f },
	m_SpriteComponent{ GetOwner()->GetComponent<SpriteComponent>() }
{
	TeleportToStartTile();
	AddMovementSrites();

	assert(m_TileManagerComponent);
	assert(m_TrainerComponent);
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
				m_SpriteComponent->SetSprite("Up");
				break;
			case Direction::Right:
				m_TargetPosition.emplace(currentPosition.x + static_cast<int>(m_TileManagerComponent->GetTileSize()), currentPosition.y);
				m_SpriteComponent->SetSprite("Right");
				// TODO: make sprite flipper helper function
				{
					const Transform spriteRenderOffset{ m_SpriteComponent->GetCurrentSprite()->RenderOffset.value() };
					m_SpriteComponent->GetCurrentSprite()->RenderOffset.value().SetScale(glm::vec2{ std::abs(spriteRenderOffset.GetScale().x), spriteRenderOffset.GetScale().y });
					m_SpriteComponent->GetCurrentSprite()->RenderOffset.value().SetPosition(glm::ivec2{ -std::abs(spriteRenderOffset.GetPosition().x), spriteRenderOffset.GetPosition().y});
				}
				break;
			case Direction::Down:
				m_TargetPosition.emplace(currentPosition.x, currentPosition.y - static_cast<int>(m_TileManagerComponent->GetTileSize()));
				m_SpriteComponent->SetSprite("Down");
				break;
			case Direction::Left:
				m_TargetPosition.emplace(currentPosition.x - static_cast<int>(m_TileManagerComponent->GetTileSize()), currentPosition.y);
				m_SpriteComponent->SetSprite("Right");
				
				{
					const Transform spriteRenderOffset{ m_SpriteComponent->GetCurrentSprite()->RenderOffset.value() };
					m_SpriteComponent->GetCurrentSprite()->RenderOffset.value().SetScale(glm::vec2{ -std::abs(spriteRenderOffset.GetScale().x), spriteRenderOffset.GetScale().y });
					m_SpriteComponent->GetCurrentSprite()->RenderOffset.value().SetPosition(glm::ivec2{ std::abs(spriteRenderOffset.GetPosition().x), spriteRenderOffset.GetPosition().y });
				}
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
	m_SpriteComponent->SetPaused(true);

#ifdef _DEBUG
	glm::ivec2 newTileIndices{ m_TileManagerComponent->GetTileIndices(newPosition) };
	std::cout << std::format("({}, {})", newTileIndices.x, newTileIndices.y) << std::endl;
#endif // DEBUG

	m_TileManagerComponent->CheckForBattle(m_TrainerComponent);
}

void MovementComponent::AddMovementSrites()
{
	assert(m_SpriteComponent);
	
	// TODO: take a look at how hardcoded values can be avoided here
	constexpr int frames{ 3 };
	constexpr glm::vec2 feetOffsetUpDown{ 0, 5 };
	constexpr glm::vec2 feetOffsetRight{ -2, 5 };
	const std::chrono::milliseconds frameTime{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(m_TileManagerComponent->GetTileSize() / (m_Speed * frames))) };
	const float renderScale{ m_TileManagerComponent->GetRenderScale() };

	const SpriteInformation downSpriteInformation
	{ 
		ResourceManager::Instance()->LoadSprite("Character/Down.png", frames, 3, 1),
		frameTime,
		Transform{ glm::ivec2{ feetOffsetUpDown * renderScale }, 0, glm::vec2{ renderScale } }
	};
	m_SpriteComponent->AddSprite("Down", downSpriteInformation);

	const SpriteInformation rightSpriteInformation
	{
		ResourceManager::Instance()->LoadSprite("Character/Right.png", frames, 3, 1),
		frameTime,
		Transform{ glm::ivec2{ feetOffsetRight * renderScale }, 0, glm::vec2{ renderScale } }
	};
	m_SpriteComponent->AddSprite("Right", rightSpriteInformation);

	const SpriteInformation upSpriteInformation
	{
		ResourceManager::Instance()->LoadSprite("Character/Up.png", frames, 3, 1),
		frameTime,
		Transform{ glm::ivec2{ feetOffsetUpDown * renderScale }, 0, glm::vec2{ renderScale } }
	};
	m_SpriteComponent->AddSprite("Up", upSpriteInformation);

	m_SpriteComponent->SetSprite("Down");
	m_SpriteComponent->SetPaused(true);
	m_SpriteComponent->SetLoop(false);
}