#include <exception>

#include "MoveCommand.h"
#include "GameObject.h"
#include "MovementComponent.h"

using namespace Minigin;

MoveCommand::MoveCommand(MovementComponent* component, MovementComponent::Direction direction) :
	GameObjectCommand{ component->GetOwner() },
	m_Direction{ direction },
	m_MovementComponent{ component }
{
	if (GetGameObject() == nullptr)
	{
		throw std::exception{ "MoveCommand::MoveCommand() - Move command has invalid game object" };
	}

	if (m_MovementComponent == nullptr)
	{
		throw std::exception{ "MoveCommand::MoveCommand() - Move command has invalid tank component." };
	}
}

void MoveCommand::Execute()
{
	m_MovementComponent->Move(m_Direction);
}