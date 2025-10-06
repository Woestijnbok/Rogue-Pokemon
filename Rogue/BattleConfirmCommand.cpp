#include "BattleConfirmCommand.h"

// Core
#include "Command.h"

// Other
#include "BattleManagerComponent.h"

BattleConfirmCommand::BattleConfirmCommand(BattleManagerComponent* battleManager) :
	GameObjectCommand{ battleManager->GetOwner() },
	m_BattleManager{ battleManager }
{

}

void BattleConfirmCommand::Execute()
{
	m_BattleManager->Confirm();
}