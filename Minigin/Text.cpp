#include "Text.h"
#include "Font.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"

using namespace Minigin;

Text::Text(const std::string& text, std::shared_ptr<Font> font) :
	m_NeedsUpdate{ true },
	m_Text{ text },
	m_Font{ std::move(font) },
	m_Texture{ Renderer::Instance()->CreateTexture(m_Font.get(), m_Text) }		
{

}

Text::~Text() = default;

void Text::Update()
{
	if (m_NeedsUpdate)
	{
		m_Texture.reset(Renderer::Instance()->CreateTexture(m_Font.get(), m_Text));
		m_NeedsUpdate = false;
	}
}

void Text::Render(const Transform& transform) const
{
	m_Texture->Render(transform);
}

void Text::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

std::shared_ptr<Font> Text::GetFont() const
{
	return m_Font;
}

glm::ivec2 Minigin::Text::GetSize()
{
	if (m_NeedsUpdate)	
	{
		m_Texture.reset(Renderer::Instance()->CreateTexture(m_Font.get(), m_Text));	
		m_NeedsUpdate = false;	
	}

	return m_Texture->GetSize();
}
