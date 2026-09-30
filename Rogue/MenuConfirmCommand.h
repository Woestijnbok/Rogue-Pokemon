#pragma once

// Core
#include "Command.h"

// Other
#include "Enums.h"

class TrainerComponent;

/*
* Command that will change the selected move.
*/
class MenuConfirmCommand final : public Minigin::GameObjectCommand
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

private:
	// Cached trainer component
	TrainerComponent* m_Trainer;

};