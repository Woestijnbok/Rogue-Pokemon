#include "MenuConfirmCommand.h"

// Core
#include "Command.h"

// Other
#include "TrainerComponent.h"

MenuConfirmCommand::MenuConfirmCommand(TrainerComponent* trainer) :
	GameObjectCommand{ trainer->GetOwner() },
	m_Trainer{ trainer }
{

}

void MenuConfirmCommand::Execute()
{
	m_Trainer->ConfirmPickup();
}