#include "MovementCommand.h"

MovementCommand::MovementCommand(MovementComponent* movementComponent, Direction direction) :
	Minigin::ComponentCommand<MovementComponent>{ movementComponent },
	m_Direction{ direction }
{
	
}

void MovementCommand::Execute()
{
	GetComponent()->Move(m_Direction);
}