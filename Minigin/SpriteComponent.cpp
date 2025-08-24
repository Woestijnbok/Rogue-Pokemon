#include <exception>

#include "SpriteComponent.h"
#include "Sprite.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TimeManager.h"

using namespace Minigin;

SpriteComponent::SpriteComponent(GameObject* owner) :
	Component{ owner },
	m_Sprites{},
	m_CurrentSpriteName{},
	m_Paused{ false },
	m_Loop{ true },
	m_AccumulatedFrameTime{},
	m_CurrentFrame{ 0 },
	m_RenderOffset{ std::nullopt },
	m_RenderScale{ std::nullopt }
{

}

SpriteComponent::~SpriteComponent() = default;

void SpriteComponent::Update()
{
	if (!m_CurrentSpriteName.empty() and !m_Paused)
	{
		m_AccumulatedFrameTime += std::chrono::duration_cast<std::chrono::milliseconds>(TimeManager::Instance()->GetDeltaTime());

		const std::chrono::milliseconds frameTime{ m_Sprites.at(m_CurrentSpriteName).second };

		if (m_AccumulatedFrameTime >= frameTime)
		{
			m_CurrentFrame = (m_CurrentFrame + 1) % m_Sprites.at(m_CurrentSpriteName).first->GetFrames();
			m_AccumulatedFrameTime -= frameTime;
			if (m_CurrentFrame == 0)
			{
				m_Paused = true;
			}
			
		}
	}
}

void SpriteComponent::Render() const		
{
	if (!m_CurrentSpriteName.empty())
	{
		Transform renderTransform{ GetOwner()->GetWorldTransform() };
		if (m_RenderOffset.has_value())
		{
			renderTransform.SetPosition(renderTransform.GetPosition() + m_RenderOffset.value());
		}
		if (m_RenderScale.has_value())
		{
			renderTransform.SetScale(m_RenderScale.value());
		}

		Renderer::Instance()->RenderSprite(*m_Sprites.at(m_CurrentSpriteName).first.get(), m_CurrentFrame, renderTransform);
	}
}

void SpriteComponent::AddSprite(const std::shared_ptr<Sprite>& sprite, const std::chrono::milliseconds frameTime, const std::string& name)
{
	m_Sprites.emplace(name, std::make_pair(sprite, frameTime));
}

void SpriteComponent::SetSprite(const std::string& name, bool loop)
{
	if (m_Sprites.find(name) != m_Sprites.end())
	{
		m_CurrentSpriteName = name;
		m_Loop = loop;
		Reset();
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

void Minigin::SpriteComponent::SetPaused(bool paused)
{
	m_Paused = paused;
}

void Minigin::SpriteComponent::SetRenderOffset(const glm::ivec2& offset)
{
	m_RenderOffset = offset;
}

void Minigin::SpriteComponent::SetRenderScale(const glm::vec2& scale)
{
	m_RenderScale = scale;
}

glm::ivec2 Minigin::SpriteComponent::GetRenderOffset() const
{
	return m_RenderOffset.has_value() ? m_RenderOffset.value() : glm::ivec2{};
}

glm::vec2 Minigin::SpriteComponent::GetRenderScale() const
{
	return m_RenderScale.has_value() ? m_RenderScale.value() : glm::vec2{};
}

void Minigin::SpriteComponent::ClearRenderScale()
{
	m_RenderScale.reset();
}

void Minigin::SpriteComponent::ClearRenderOffset()
{
	m_RenderOffset.reset();
}

void Minigin::SpriteComponent::Reset()
{
	m_CurrentFrame = 0;
	m_Paused = false;
	m_AccumulatedFrameTime = std::chrono::milliseconds{ 0 };
}