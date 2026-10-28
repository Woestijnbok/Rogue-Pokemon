#include "SelectMoveCommand.h"

SelectMoveCommand::SelectMoveCommand(BattleManagerComponent* battleManager, Direction direction) :
	Minigin::ComponentCommand<BattleManagerComponent>{ battleManager },
	m_Direction{ direction }
{
	
}

void SelectMoveCommand::Execute()
{
	GetComponent()->ChangeSelectedMove(m_Direction);
}