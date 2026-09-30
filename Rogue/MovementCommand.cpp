#include "MovementCommand.h"

// Libraries
#include <cassert>

// Core
#include "GameObject.h"

// Components
#include "MovementComponent.h"

using namespace Minigin;

MovementCommand::MovementCommand(MovementComponent* movementComponent, Direction direction) :
	ComponentCommand{ movementComponent },
	m_Direction{ direction },
	m_MovementComponent{ movementComponent }
{
	assert(m_MovementComponent);
}

void MovementCommand::Execute()
{
	m_MovementComponent->Move(m_Direction);
}