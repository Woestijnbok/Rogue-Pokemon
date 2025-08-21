#include <stdexcept>

#include "GameObject.h"
#include "Renderer.h"
#include "Scene.h"
#include "InputManager.h"

using namespace Minigin;

GameObject::GameObject(Scene* scene, const std::string& name) :
	ControllableObject{},
	ObjectController<Component>{},
	m_LocalTransform{},
	m_WorldTransform{},
	m_Components{},
	m_Children{},
	m_Parent{},
	m_Scene{ scene },
	m_Name{ name }
{

}

Minigin::GameObject::~GameObject()
{
	InputManager::Instance()->RemoveInputActions(this);
}

void GameObject::FlagWorldTransform()
{
	m_WorldTransform.first = false;
	for (GameObject* child : m_Children) child->FlagWorldTransform();
}

const Transform& GameObject::GetWorldTransform()
{
	if (!m_WorldTransform.first)
	{
		if (m_Parent == nullptr)
		{
			m_WorldTransform.second = m_LocalTransform;
		}
		else
		{
			const Transform parentWorld{ m_Parent->GetWorldTransform() };

			const Transform newWorld
			{
				parentWorld.GetPosition() + m_LocalTransform.GetPosition(),
				parentWorld.GetRotation() + m_LocalTransform.GetRotation(),
				parentWorld.GetScale() * m_LocalTransform.GetScale()
			};

			m_WorldTransform.second = newWorld;		
		}

		m_WorldTransform.first = true;
	}

	return m_WorldTransform.second;
}

const Transform& GameObject::GetLocalTransform() const
{
	return m_LocalTransform;
}

void GameObject::SetLocalTransform(const Transform& transform)
{
	m_LocalTransform = transform;
	FlagWorldTransform();
}

void GameObject::SetLocalPosition(const glm::ivec2& position)
{
	m_LocalTransform.SetPosition(position);
	FlagWorldTransform();
}

void GameObject::SetLocalRotation(int rotation)
{
	m_LocalTransform.SetRotation(rotation);
	FlagWorldTransform();
}

void GameObject::SetLocalScale(const glm::vec2& scale)
{
	m_LocalTransform.SetScale(scale);
	FlagWorldTransform();
}

GameObject* GameObject::GetParent() const
{
	return m_Parent;
}

void GameObject::SetParent(GameObject* parent, bool keepWorldTransform)
{
	// Check if the new parent is valid
	if ((m_Parent != parent) and 
		(std::ranges::none_of(m_Children, [parent](GameObject* child) -> bool { return child == parent; })) and
		(parent != this))
	{
		// Fix our world and local position
		if (parent == nullptr)
		{
			SetLocalTransform(GetWorldTransform());
		}
		else
		{
			if (keepWorldTransform)
			{
				const Transform oldWorld{ GetWorldTransform() };
				const Transform newParentWorld{ parent->GetWorldTransform() };
				const Transform newLocal
				{
					oldWorld.GetPosition() - newParentWorld.GetPosition(),
					oldWorld.GetRotation() - newParentWorld.GetRotation(),	
					oldWorld.GetScale() / newParentWorld.GetScale()	
				};

				SetLocalTransform(newLocal);
			}
		}

		// Remove itself from the previous parent
		if (m_Parent != nullptr)
		{
			auto it = std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this);

			if (it != m_Parent->m_Children.end())
			{
				m_Parent->m_Children.erase(it);
			}
		}

		// Set the given parent
		m_Parent = parent;

		// Add itself as a child to the given parent
		if (parent != nullptr)
		{
			parent->m_Children.push_back(this);
		}
	}
	else
	{
		throw std::runtime_error("GameObject::SetParent() - Invalid parent given");
	}
}

size_t GameObject::GetChildCount() const
{
	return m_Children.size();
}

GameObject* GameObject::GetChild(size_t index) const 
{
	return m_Children.at(index);
}

const std::string& GameObject::GetName() const
{
	return m_Name;
}

Scene* GameObject::GetScene() const
{
	return m_Scene;
}
