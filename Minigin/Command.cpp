#include "Command.h"
#include "Scene.h"

using namespace Minigin;

void Minigin::Command::Execute()
{

}

void Minigin::Command::Execute(const glm::vec2& /*axis*/)
{

}

GameObjectCommand::GameObjectCommand(GameObject* gameObject) :
	Command(),
	m_GameObject{ gameObject }
{

}

bool Minigin::GameObjectCommand::IsValid() const
{
	return m_GameObject->GetScene()->GetStatus() == ControllableObject::Status::Enabled and m_GameObject->GetStatus() == ControllableObject::Status::Enabled;
}

GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}