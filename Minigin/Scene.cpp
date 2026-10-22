#include "Scene.h"

#include "Component.h"

#include <algorithm>

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

void Scene::Render() const
{
	std::vector<Component*> componentsToRender{};
	for (GameObject* object : GetEnabledObjects())
	{
		for (Component* component : object->GetComponents())
		{
			componentsToRender.push_back(component);
		}
	}

	std::ranges::stable_sort(componentsToRender,
		[](Component* a, Component* b)
		{
			return static_cast<uint8_t>(a->GetRenderOrder()) < static_cast<uint8_t>(b->GetRenderOrder());
		}
	);

	for (Component* component : componentsToRender)
	{
		component->Render();
	}
}