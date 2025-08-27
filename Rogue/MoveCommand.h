#pragma once

// Core
#include "Command.h"

// Other
#include "Enums.h"

class MovementComponent;

/*
* Command that will call move on the movement component in the given direction.
*/
class MoveCommand final : public Minigin::GameObjectCommand
{
public:
	/*
	* Constructs command
	* 
	* @param movementComponent: The movement component that will be used to call move logic.
	* @param direction: The move direction used in move calls.
	*/
	MoveCommand(MovementComponent* movementComponent, Direction direction);
	virtual ~MoveCommand() = default;

	MoveCommand(const MoveCommand&) = delete;
	MoveCommand(MoveCommand&&) = delete;
	MoveCommand& operator= (const MoveCommand&) = delete;
	MoveCommand& operator= (const MoveCommand&&) = delete;

	/*
	* Will try start movement using the cached movement component.
	*/
	virtual void Execute() override;

private:
	// Move direction for all move calls
	Direction m_Direction;
	// Cached movement component
	MovementComponent* m_MovementComponent;

};