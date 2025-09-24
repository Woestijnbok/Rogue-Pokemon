#include "SelectMoveCommand.h"

// Libraries
#include <cassert>

// Core
#include "GameObject.h"

// Components
#include "BattleManagerComponent.h"

using namespace Minigin;

SelectMoveCommand::SelectMoveCommand(BattleManagerComponent* battleManager, Direction direction) :
	GameObjectCommand{ battleManager->GetOwner() },
	m_Direction{ direction },
	m_BattleManager{ battleManager }
{
	assert(m_BattleManager);
}

void SelectMoveCommand::Execute()
{
	m_BattleManager->ChangeSelectedMove(m_Direction);
}