#include "TextComponent.h"
#include "Text.h"
#include "Font.h"
#include "GameObject.h"	
#include "Renderer.h"

using namespace Minigin;

TextComponent::TextComponent(GameObject* owner, const std::string& text, Font* font, const Color& color) :
	Component{ owner },
	m_Text{ std::make_unique<Text>(text, font, color) }
{

}

Minigin::TextComponent::~TextComponent() = default;

void TextComponent::Render() const
{
	Renderer::Instance()->RenderText(*m_Text.get(), GetOwner()->GetWorldTransform());	
}

void TextComponent::SetText(const std::string& text)
{
	m_Text->SetText(text);
}

Font* TextComponent::GetFont()
{
	return m_Text->GetFont();	
}