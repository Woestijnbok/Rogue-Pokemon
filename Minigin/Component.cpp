#include <stdexcept>

#include "Component.h"

using namespace Minigin;

Component::Component(GameObject* owner) :
	ControllableObject{},
	m_Owner{ owner }
{
	if (owner == nullptr) throw std::runtime_error("Owner::Owner() - Invalid owner passed");
}

void Component::Update()
{

}

void Component::FixedUpdate()
{

}

void Component::LateUpdate()
{

}

void Component::Render() const
{

}

GameObject* Component::GetOwner() const
{
	return m_Owner;
}