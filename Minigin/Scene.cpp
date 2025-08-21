#include "Scene.h"

using namespace Minigin;

Scene::Scene(const std::string& name) : 
	ControllableObject{},
	ObjectController<GameObject>{},	
	m_Name{ name }	
{

}

GameObject* Scene::CreateGameObject(const std::string& name, bool enabled)	
{
	if (GetGameObject(name) != nullptr)
	{
		throw std::exception{ "Scene::CreateGameObject() - Already have a game object with the same." };
	}

	GameObject* gameObject{ new GameObject{ this, name } };	
	if (!enabled) gameObject->SetStatus(ControllableObject::Status::Disabled);
	AddControllableObject(gameObject);
	
	return gameObject;
}

GameObject* Minigin::Scene::GetGameObject(const std::string& name) const
{
	return GetControllableObject
	(
		[&name](GameObject* object) -> bool
		{
			return object->GetName() == name;
		}
	);
}

const std::string& Scene::GetName() const
{
	return m_Name;
}