#include "UseItemCommand.h"

UseItemCommand::UseItemCommand(TrainerComponent* trainerComponent, ItemType item) :
	Minigin::ComponentCommand<TrainerComponent>{ trainerComponent },
	m_ItemType{ item }
{

}

void UseItemCommand::Execute()
{
	GetComponent()->TryUseItem(m_ItemType);
}