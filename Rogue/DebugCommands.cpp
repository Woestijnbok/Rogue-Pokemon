#include "DebugCommands.h"

// Components
#include "BattleManagerComponent.h"

using namespace Minigin;

SkipBattleCommand::SkipBattleCommand(BattleManagerComponent* battleManager) :
	Command{},
	m_BattleManager{ battleManager }
{

}

void SkipBattleCommand::Execute()
{
	if (m_BattleManager->InBattle())
	{
		m_BattleManager->EndBattle();
	}
}