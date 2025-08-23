#pragma once

#include "Command.h"

class TrainerComponent;

class MoveCommand final : public Minigin::GameObjectCommand
{
public:
	enum class Direction
	{
		Up,
		Right,
		Down,
		Left
	};


	MoveCommand(TrainerComponent* trainer, Direction direction);
	virtual ~MoveCommand() = default;

	MoveCommand(const MoveCommand&) = delete;
	MoveCommand(MoveCommand&&) = delete;
	MoveCommand& operator= (const MoveCommand&) = delete;
	MoveCommand& operator= (const MoveCommand&&) = delete;

	virtual void Execute() override;

private:
	const Direction m_Direction;
	TrainerComponent* m_Trainer;

};