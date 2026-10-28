#include "MenuConfirmCommand.h"

MenuConfirmCommand::MenuConfirmCommand(TrainerComponent* trainer) :
	Minigin::ComponentCommand<TrainerComponent>{ trainer }
{

}

void MenuConfirmCommand::Execute()
{
	GetComponent()->ConfirmPickup();
}