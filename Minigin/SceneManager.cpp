#include "SceneManager.h"

using namespace Minigin;

Scene* SceneManager::CreateScene(const std::string& name, bool enabled)
{
	Scene* scene{ new Scene{name} };
	if (!enabled) scene->SetStatus(ControllableObject::Status::Disabled);
	AddControllableObject(scene);

	return scene;
}

Scene* SceneManager::GetScene(const std::string& name) const
{
	Scene* scene	
	{
		GetControllableObject
		(
			[&name](const Scene* element) -> bool
			{
				return element->GetName() == name;
			}
		)
	};

	return scene;	
}

void Minigin::SceneManager::RemoveScene(const std::string name)
{
	RemoveControllableObjects
	(
		[&name](Scene* scene) -> bool
		{
			return scene->GetName() == name;
		}
	);
}

void Minigin::SceneManager::IsolateScene(const std::string& name)
{
	RemoveControllableObjects	
	(
		[&name](Scene* scene) -> bool
		{
			return scene->GetName() != name;
		}
	);
}