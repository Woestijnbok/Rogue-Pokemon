#pragma once

// Core
#include "Command.h"

// Other
#include "Enums.h"

class MovementComponent;

/*
* Command that will call move on the movement component in the given direction.
*/
class MovementCommand final : public Minigin::GameObjectCommand
{
public:
	/*
	* Constructs command
	* 
	* @param movementComponent: The movement component that will be used to call move logic.
	* @param direction: The move direction used in move calls.
	*/
	MovementCommand(MovementComponent* movementComponent, Direction direction);
	virtual ~MovementCommand() = default;

	MovementCommand(const MovementCommand&) = delete;
	MovementCommand(MovementCommand&&) = delete;
	MovementCommand& operator= (const MovementCommand&) = delete;
	MovementCommand& operator= (const MovementCommand&&) = delete;

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