#include <exception>

#include "SpriteComponent.h"
#include "Sprite.h"
#include "Renderer.h"
#include "GameObject.h"

using namespace Minigin;

SpriteComponent::SpriteComponent(GameObject* owner) :
	Component{ owner },
	m_Sprites{},
	m_CurrentSpriteName{},
	m_LastTimePoint{},
	m_Frame{ 0 },
	m_UpdateTimePoint{ true }
{

}

SpriteComponent::~SpriteComponent() = default;

void SpriteComponent::Update()
{
	if (m_CurrentSpriteName.empty()) return;

	if (m_UpdateTimePoint)
	{
		m_UpdateTimePoint = false;	
		m_LastTimePoint = std::chrono::steady_clock::now();
	}

	const auto timeDifference{ std::chrono::steady_clock::now() - m_LastTimePoint };			

	if (timeDifference >= m_Sprites.at(m_CurrentSpriteName).second)
	{
		m_LastTimePoint = std::chrono::steady_clock::now();	
		m_Frame = (m_Frame + 1) % m_Sprites.at(m_CurrentSpriteName).first->GetFrames();	
	}
}

void SpriteComponent::Render() const		
{
	if (m_CurrentSpriteName.empty()) return;	

	Renderer::Instance()->RenderSprite(*m_Sprites.at(m_CurrentSpriteName).first.get(), m_Frame, GetOwner()->GetWorldTransform());	
}

void SpriteComponent::AddSprite(const std::shared_ptr<Sprite>& sprite, const std::chrono::milliseconds frameTime, const std::string& name)
{
	m_Sprites.emplace(name, std::make_pair(sprite, frameTime));
}

void SpriteComponent::SetSprite(const std::string& name)
{
	if (m_Sprites.find(name) != m_Sprites.end())
	{
		m_CurrentSpriteName = name;
		m_UpdateTimePoint = true;
		m_Frame = 0;
	}
	else
	{
		throw std::exception("SpriteComponent::SetSprite() - No sprite found with the given name.");
	}
}

std::shared_ptr<Sprite> SpriteComponent::GetSprite(const std::string& name) const
{
	return m_Sprites.at(name).first;
}

void Minigin::SpriteComponent::Reset()
{
	m_Frame = 0;
}