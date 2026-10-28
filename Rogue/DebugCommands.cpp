#include "DebugCommands.h"

SkipBattleCommand::SkipBattleCommand(BattleManagerComponent* battleManager) :
	Minigin::ComponentCommand<BattleManagerComponent>{ battleManager }
{

}

void SkipBattleCommand::Execute()
{
	if (GetComponent()->InBattle())
	{
		GetComponent()->EndBattle();
	}
}