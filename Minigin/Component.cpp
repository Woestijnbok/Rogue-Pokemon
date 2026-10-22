#include <stdexcept>

#include "Component.h"

using namespace Minigin;

Component::Component(GameObject* owner) :
	ControllableObject{},
	m_Owner{ owner },
	m_RenderOrder{ RenderOrder::World }
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

RenderOrder Minigin::Component::GetRenderOrder() const
{
	return m_RenderOrder;
}

void Minigin::Component::SetRenderOrder(const RenderOrder order)
{
	m_RenderOrder = order;
}

GameObject* Component::GetOwner() const
{
	return m_Owner;
}