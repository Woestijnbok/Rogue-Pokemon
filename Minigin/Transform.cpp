#include "Transform.h"

using namespace Minigin;

Transform::Transform() :
	m_Position{},
	m_Rotation{},
	m_Scale{ 1.0f, 1.0f }
{

}

Transform::Transform(glm::ivec2 position, int rotation, glm::vec2 scale) :
	m_Position{ position },
	m_Rotation{ rotation },
	m_Scale{ scale }
{

}	

Transform::Transform(const Transform& rhs) :
	m_Position{ rhs.m_Position },
	m_Rotation{ rhs.m_Rotation },
	m_Scale{ rhs.m_Scale }
{

}

Transform::Transform(Transform&& rhs) noexcept :
	m_Position{ rhs.m_Position },	
	m_Rotation{ rhs.m_Rotation },	
	m_Scale{ rhs.m_Scale }	
{

}

Transform& Transform::operator=(const Transform& rhs)
{
	m_Position = rhs.m_Position;
	m_Rotation = rhs.m_Rotation;
	m_Scale = rhs.m_Scale;

	return *this;
}

Transform& Transform::operator=(const Transform&& rhs) noexcept
{
	m_Position = rhs.m_Position;
	m_Rotation = rhs.m_Rotation;
	m_Scale = rhs.m_Scale;

	return *this;
}


const glm::ivec2& Transform::GetPosition() const
{
	return m_Position; 
}

int Transform::GetRotation() const
{
	return m_Rotation;
}

const glm::vec2& Transform::GetScale() const
{
	return m_Scale;
}

void Transform::SetPosition(const glm::ivec2& position)
{
	m_Position = position;
}

void Transform::SetRotation(int rotation)
{
	m_Rotation = rotation;
}

void Transform::SetScale(const glm::vec2& scale)
{
	m_Scale = scale;
}