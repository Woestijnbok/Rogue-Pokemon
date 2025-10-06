#pragma once

// Core
#include "Command.h"

// Other
#include "Enums.h"

class BattleManagerComponent;

/*
* Command that will change the selected move.
*/
class BattleConfirmCommand final : public Minigin::GameObjectCommand
{
public:
	/*
	* Constructs command
	*
	* @param movementComponent: The movement component that will be used to call move logic.
	* @param direction: The move direction used in move calls.
	*/
	BattleConfirmCommand(BattleManagerComponent* battleManager);
	virtual ~BattleConfirmCommand() = default;

	BattleConfirmCommand(const BattleConfirmCommand&) = delete;
	BattleConfirmCommand(BattleConfirmCommand&&) = delete;
	BattleConfirmCommand& operator= (const BattleConfirmCommand&) = delete;
	BattleConfirmCommand& operator= (const BattleConfirmCommand&&) = delete;

	/*
	* Will confirm the current state of the battle manager.
	*/
	virtual void Execute() override;

private:
	// Cached movement component
	BattleManagerComponent* m_BattleManager;

};