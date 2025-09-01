#include "Text.h"
#include "Renderer.h"
#include "Transform.h"
#include "ResourceManager.h"

using namespace Minigin;

Text::Text(const std::string& text, Font* font, const Color& color) :
	m_NeedsUpdate{ false },
	m_Text{ text },
	m_Font{ font },
	m_Texture{ Renderer::Instance()->CreateTexture(font, text, color)},
	m_Color{ color }
{

}

void Text::Update()
{
	if (m_NeedsUpdate)
	{
		m_Texture.reset(Renderer::Instance()->CreateTexture(m_Font, m_Text, m_Color));
		m_NeedsUpdate = false;
	}
}

void Text::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

const std::string& Text::GetText() const
{
	return m_Text;
}

Font* Text::GetFont() const
{
	return m_Font;
}

Texture* Minigin::Text::GetTexture()
{
	if (m_NeedsUpdate)
	{
		Update();
	}

	return m_Texture.get();
}