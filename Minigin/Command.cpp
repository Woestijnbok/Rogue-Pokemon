#include "Command.h"
#include "GameObject.h"
#include "Scene.h"
#include "Component.h"

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

Minigin::ComponentCommand::ComponentCommand(Component* component) :
	GameObjectCommand(component->GetOwner()),
	m_Component{ component }
{

}

bool Minigin::ComponentCommand::IsValid() const
{
	return GameObjectCommand::IsValid() and m_Component->GetStatus() == ControllableObject::Status::Enabled;
}

Component* Minigin::ComponentCommand::GetComponent() const
{
	return m_Component;
}