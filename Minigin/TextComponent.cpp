#include "TextComponent.h"
#include "Text.h"
#include "Font.h"
#include "GameObject.h"	

using namespace Minigin;

TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font) :
	Component{ owner },
	m_Text{ std::make_unique<Text>(text, font) }
{

}

Minigin::TextComponent::~TextComponent() = default;

void TextComponent::Update()
{
	m_Text->Update();	
}

void TextComponent::Render() const
{
	m_Text->Render(GetOwner()->GetWorldTransform());	
}

void TextComponent::SetText(const std::string& text)
{
	m_Text->SetText(text);
}

std::shared_ptr<Font> TextComponent::GetFont()
{
	return m_Text->GetFont();	
}
