#include "MoveCommand.h"

// Libraries
#include <cassert>

// Core
#include "GameObject.h"

// Components
#include "MovementComponent.h"

using namespace Minigin;

MoveCommand::MoveCommand(MovementComponent* movementComponent, Direction direction) :
	GameObjectCommand{ movementComponent->GetOwner() },
	m_Direction{ direction },
	m_MovementComponent{ movementComponent }
{
	assert(m_MovementComponent);
}

void MoveCommand::Execute()
{
	m_MovementComponent->Move(m_Direction);
}