#pragma once

// Core
#include "Command.h"
#include "BattleManagerComponent.h"

/*
* Command that will change the selected move.
*/
class SelectMoveCommand final : public Minigin::ComponentCommand<BattleManagerComponent>
{
public:
	/*
	* Constructs command
	*
	* @param movementComponent: The movement component that will be used to call move logic.
	* @param direction: The move direction used in move calls.
	*/
	SelectMoveCommand(BattleManagerComponent* battleManager, Direction direction);
	virtual ~SelectMoveCommand() = default;

	SelectMoveCommand(const SelectMoveCommand&) = delete;
	SelectMoveCommand(SelectMoveCommand&&) = delete;
	SelectMoveCommand& operator= (const SelectMoveCommand&) = delete;
	SelectMoveCommand& operator= (const SelectMoveCommand&&) = delete;

	/*
	* Will try and change the the selected move.
	*/
	virtual void Execute() override;

private:
	// Move direction for all move calls
	Direction m_Direction;

};