#include "FPSCounterComponent.h"
#include "Text.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

using namespace Minigin;

FPSCounterComponent::FPSCounterComponent(GameObject* owner) :
	Component{ owner },
	m_LastTimePoint{},
	m_FrameCounter{ 0 },
	m_Text{ std::make_unique<Text>("0.0 FPS", ResourceManager::Instance()->LoadFont("Lingua.otf", "Lingua", 36), Color::Black) },
	m_Running{ false }
{
	
}

FPSCounterComponent::~FPSCounterComponent() = default;

void FPSCounterComponent::Update()
{
	if (!m_Running) 
	{
		m_Running = true;
		m_LastTimePoint = std::chrono::steady_clock::now();	
	}

	++m_FrameCounter;
	const auto timeDifference{ std::chrono::steady_clock::now() - m_LastTimePoint };	

	if (timeDifference >= std::chrono::seconds(1))
	{
		std::ostringstream stream{};
		stream << m_FrameCounter << " FPS";
		m_Text->SetText(stream.str());		

		m_FrameCounter = 0;
		m_LastTimePoint = std::chrono::steady_clock::now();	
	}	
}

void FPSCounterComponent::Render() const
{
	Renderer::Instance()->RenderText(*m_Text.get(), GetOwner()->GetWorldTransform());
}