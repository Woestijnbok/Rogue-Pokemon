#include "BattleConfirmCommand.h"

BattleConfirmCommand::BattleConfirmCommand(BattleManagerComponent* battleManager) :
	Minigin::ComponentCommand<BattleManagerComponent>{ battleManager }
{

}

void BattleConfirmCommand::Execute()
{
	GetComponent()->Confirm();
}