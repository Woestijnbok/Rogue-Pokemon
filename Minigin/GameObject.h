#pragma once

#include <algorithm>
#include <concepts>
#include <type_traits>
#include <memory>
#include <vector>
#include <string>

#include "Transform.h"
#include "Component.h"
#include "ObjectController.h"

namespace Minigin
{
	class Scene;

	template<typename Type>
	concept IsComponent = std::is_base_of_v<Component, Type>;

	class GameObject final : public ControllableObject, public ObjectController<Component>
	{
	public:
		explicit GameObject(Scene* scene, const std::string& name);
		~GameObject();

		GameObject(const GameObject& other) = delete;	
		GameObject(GameObject&& other) noexcept = delete;	
		GameObject& operator=(const GameObject& other) = delete;	
		GameObject& operator=(GameObject&& other) noexcept = delete;	

		void SetLocalTransform(const Transform& transform);
		void SetLocalPosition(const glm::ivec2& position);
		void SetLocalRotation(int rotation);
		void SetLocalScale(const glm::vec2& scale);	
		const Transform& GetWorldTransform();
		const Transform& GetLocalTransform() const;
		GameObject* GetParent() const;
		void SetParent(GameObject* parent, bool keepWorldTransform = true);
		size_t GetChildCount() const;
		GameObject* GetChild(size_t index) const;
		const std::string& GetName() const;
		Scene* GetScene() const;

		template<typename Type, typename... Arguments>
			requires IsComponent<Type>
		Type* CreateComponent(Arguments&&... arguments);

		template<typename Type>
			requires IsComponent<Type>
		Type* GetComponent() const;

	private:
		const std::string m_Name;
		Transform m_LocalTransform;
		std::pair<bool, Transform> m_WorldTransform;
		std::vector<std::unique_ptr<Component>> m_Components;
		std::vector<GameObject*> m_Children;
		GameObject* m_Parent;
		Scene* const m_Scene;

		void FlagWorldTransform();
	};

	template<typename Type, typename ...Arguments>
		requires IsComponent<Type>
	Type* GameObject::CreateComponent(Arguments && ...arguments)
	{
		if (GetComponent<Type>() == nullptr)	
		{
			Component* component{ new Type{ this, std::forward<Arguments>(arguments)... } };
			AddControllableObject(component);

			return dynamic_cast<Type*>(component);
		}
		else return nullptr;
	}

	template<typename Type>
		requires IsComponent<Type>
	Type* GameObject::GetComponent() const
	{
		Component* component
		{
			GetControllableObject
			(
				[](Component* comp) -> bool	
				{
					return typeid(*comp) == typeid(Type);
				}
			)
		};

		return dynamic_cast<Type*>(component);
	}
}