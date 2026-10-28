#pragma once

// Core
#include "Command.h"
#include "TrainerComponent.h"

/*
* Command that will change the selected move.
*/
class MenuConfirmCommand final : public Minigin::ComponentCommand<TrainerComponent>
{
public:
	/*
	* Constructs command
	*
	* @param movementComponent: The movement component that will be used to call move logic.
	* @param direction: The move direction used in move calls.
	*/
	MenuConfirmCommand(TrainerComponent* trainer);
	virtual ~MenuConfirmCommand() = default;

	MenuConfirmCommand(const MenuConfirmCommand&) = delete;
	MenuConfirmCommand(MenuConfirmCommand&&) = delete;
	MenuConfirmCommand& operator= (const MenuConfirmCommand&) = delete;
	MenuConfirmCommand& operator= (const MenuConfirmCommand&&) = delete;

	/*
	* Will confirm the current state of the battle manager.
	*/
	virtual void Execute() override;
};