#pragma once

#include "Command.h"
#include "MovementComponent.h"

class BattleManagerComponent;

class SkipBattleCommand final : public Minigin::Command
{
public:
	SkipBattleCommand(BattleManagerComponent* battleManager);
	virtual ~SkipBattleCommand() = default;

	SkipBattleCommand(const SkipBattleCommand&) = delete;
	SkipBattleCommand(SkipBattleCommand&&) = delete;
	SkipBattleCommand& operator= (const SkipBattleCommand&) = delete;
	SkipBattleCommand& operator= (const SkipBattleCommand&&) = delete;

	virtual void Execute() override;

private:
	BattleManagerComponent* m_BattleManager;

};