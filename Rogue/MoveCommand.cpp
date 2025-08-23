#include <exception>

#include "MoveCommand.h"
#include "GameObject.h"
#include "TrainerComponent.h"

using namespace Minigin;

MoveCommand::MoveCommand(TrainerComponent* trainer, Direction direction) :
	GameObjectCommand{ trainer->GetOwner() },
	m_Direction{ direction },
	m_Trainer{ trainer }
{
	if (GetGameObject() == nullptr)
	{
		throw std::exception{ "MoveCommand::MoveCommand() - Move command has invalid game object" };
	}

	if (m_Trainer == nullptr)
	{
		throw std::exception{ "MoveCommand::MoveCommand() - Move command has invalid tank component." };
	}
}

void MoveCommand::Execute()
{
	m_Trainer->Move(m_Direction);
}