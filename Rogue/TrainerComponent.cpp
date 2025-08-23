#include "TrainerComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Texture.h"
#include "GameObject.h"
#include "TimeManager.h"

using namespace Minigin;

TrainerComponent::TrainerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_TrainerDownSprite{ new Sprite{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Character/test.png"), 16, 4, 4 } },
	m_TrainerUpSprite{ new Sprite{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Character/Up.png"), 4, 4, 1 } },
	m_TrainerRightSprite{ new Sprite{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Character/Right.png"), 4, 4, 1 } },
	m_Direction{ MoveCommand::Direction::Down }
{
	GetOwner()->SetLocalScale(glm::vec2{ 0.2f, 0.2f });
	GetOwner()->SetLocalPosition(glm::ivec2{ 100, 100 });
}

void TrainerComponent::Render() const
{
	Renderer::Instance()->RenderSprite(*m_TrainerDownSprite, 9, GetOwner()->GetWorldTransform());

	glm::ivec2 size = m_TrainerDownSprite->GetFrameSize();
	size;

	//Renderer::Instance()->RenderDebugBox(glm::ivec2{ 100, 100 }, glm::ivec2{ 150, 150 }, Color::Red, true);
}

void TrainerComponent::Move(MoveCommand::Direction direction)
{
	//if (!m_Manager->CanMove(this, direction)) return;

	const glm::ivec2 oldPosition{ GetOwner()->GetLocalTransform().GetPosition() };
	glm::ivec2 newPosition{ oldPosition };

	switch (direction)
	{
	case MoveCommand::Direction::Up:
		newPosition.y += static_cast<int>(2000.0f * Minigin::TimeManager::Instance()->GetDeltaTime().count());
		break;
	case MoveCommand::Direction::Right:
		newPosition.x += static_cast<int>(2000.0f * Minigin::TimeManager::Instance()->GetDeltaTime().count());
		break;
	case MoveCommand::Direction::Down:
		newPosition.y -= static_cast<int>(2000.0f * Minigin::TimeManager::Instance()->GetDeltaTime().count());
		break;
	case MoveCommand::Direction::Left:
		newPosition.x -= static_cast<int>(2000.0f * Minigin::TimeManager::Instance()->GetDeltaTime().count());
		break;
	default:
		break;
	}

	SetDirection(direction);
	GetOwner()->SetLocalPosition(newPosition);
}

void TrainerComponent::SetDirection(MoveCommand::Direction direction)
{
	m_Direction = direction;
}