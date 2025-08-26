#include "DebugCommands.h"
#include "BattleManagerComponent.h"

SkipBattleCommand::SkipBattleCommand(BattleManagerComponent* battleManager) :
	Command{},
	m_BattleManager{ battleManager }
{

}

void SkipBattleCommand::Execute()
{
	m_BattleManager->EndBattle();
}