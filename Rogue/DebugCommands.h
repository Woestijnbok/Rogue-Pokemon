#pragma once

// Core
#include "Command.h"
#include "BattleManagerComponent.h"

/*
* Debug command to skip the current battle.
* Will not be compiled in release modes.
*/
class SkipBattleCommand final : public Minigin::ComponentCommand<BattleManagerComponent>
{
public:
	/*
	* Constructs command with a reference to the battle manager.
	* 
	* @param battleManager: The battle manager component.
	*/
	SkipBattleCommand(BattleManagerComponent* battleManager) ;
	virtual ~SkipBattleCommand() = default;

	SkipBattleCommand(const SkipBattleCommand&) = delete;
	SkipBattleCommand(SkipBattleCommand&&) = delete;
	SkipBattleCommand& operator= (const SkipBattleCommand&) = delete;
	SkipBattleCommand& operator= (const SkipBattleCommand&&) = delete;
	
	/*
	* Will try to stop the current batlle via the battle manager.
	*/
	virtual void Execute() override;
};