#pragma once

#include "Command.h"
#include "MovementComponent.h"

class MoveCommand final : public Minigin::GameObjectCommand
{
public:
	MoveCommand(MovementComponent* component, MovementComponent::Direction direction);
	virtual ~MoveCommand() = default;

	MoveCommand(const MoveCommand&) = delete;
	MoveCommand(MoveCommand&&) = delete;
	MoveCommand& operator= (const MoveCommand&) = delete;
	MoveCommand& operator= (const MoveCommand&&) = delete;

	virtual void Execute() override;

private:
	MovementComponent::Direction m_Direction;
	MovementComponent* m_MovementComponent;

};