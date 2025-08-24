#include <exception>

#include "SpriteComponent.h"
#include "Sprite.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TimeManager.h"

using namespace Minigin;

SpriteInformation::SpriteInformation(const std::shared_ptr<Minigin::Sprite>& sprite, const std::chrono::milliseconds& frameTime, const Transform& renderOffset) :
	Sprite{ sprite },
	FrameTime{ frameTime },
	RenderOffset{ renderOffset }
{

}

SpriteInformation::SpriteInformation(const std::shared_ptr<Minigin::Sprite>& sprite, const std::chrono::milliseconds& frameTime) :
	Sprite{ sprite },
	FrameTime{ frameTime },
	RenderOffset{ std::nullopt }
{

}

SpriteComponent::SpriteComponent(GameObject* owner) :
	Component{ owner },
	m_Sprites{},
	m_CurrentSprite{ nullptr },
	m_Paused{ false },
	m_Loop{ true },
	m_AccumulatedFrameTime{},
	m_CurrentFrame{ 0 }
{

}

SpriteComponent::~SpriteComponent() = default;

void SpriteComponent::Update()
{
	if ((m_CurrentSprite != nullptr) and !m_Paused)
	{
		m_AccumulatedFrameTime += std::chrono::duration_cast<std::chrono::milliseconds>(TimeManager::Instance()->GetDeltaTime());

		const std::chrono::milliseconds frameTime{ m_CurrentSprite->FrameTime };

		if (m_AccumulatedFrameTime >= frameTime)
		{
			m_CurrentFrame = (m_CurrentFrame + 1) % m_CurrentSprite->Sprite->GetFrames();
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
	if (m_CurrentSprite != nullptr)
	{
		Transform renderTransform{ GetOwner()->GetWorldTransform() };
		if (m_CurrentSprite->RenderOffset.has_value())
		{
			const Transform renderOffset{ m_CurrentSprite->RenderOffset.value() };

			renderTransform.SetPosition(renderTransform.GetPosition() + renderOffset.GetPosition());
			renderTransform.SetRotation(renderTransform.GetRotation() + renderOffset.GetRotation());
			renderTransform.SetScale(renderTransform.GetScale() * renderOffset.GetScale());
		}

		Renderer::Instance()->RenderSprite(*m_CurrentSprite->Sprite.get(), m_CurrentFrame, renderTransform);
	}
}

void SpriteComponent::AddSprite(const std::string& name, const SpriteInformation& spriteInformation)
{
	assert(!m_Sprites.contains(name));
	m_Sprites.emplace(name, spriteInformation);
}

void SpriteComponent::SetSprite(const std::string& name)
{
	assert(m_Sprites.contains(name));

	m_CurrentSprite = &m_Sprites.at(name);
	m_Paused = false;
	Reset();
}

SpriteInformation* Minigin::SpriteComponent::GetSprite(const std::string& name)
{
	assert(m_Sprites.contains(name));

	return &m_Sprites.at(name);
}

SpriteInformation const * Minigin::SpriteComponent::GetSprite(const std::string& name) const
{
	assert(m_Sprites.contains(name));

	return &m_Sprites.at(name);
}

SpriteInformation* Minigin::SpriteComponent::GetCurrentSprite()
{
	return m_CurrentSprite;
}

SpriteInformation const * Minigin::SpriteComponent::GetCurrentSprite() const
{
	return m_CurrentSprite;
}

void Minigin::SpriteComponent::SetPaused(bool paused)
{
	m_Paused = paused;
}

void Minigin::SpriteComponent::SetLoop(bool loop)
{
	m_Loop = loop;
}

void Minigin::SpriteComponent::Reset()
{
	m_CurrentFrame = 0;
	m_AccumulatedFrameTime = std::chrono::milliseconds{ 0 };
}