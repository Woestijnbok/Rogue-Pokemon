#include "Text.h"
#include "Renderer.h"
#include "Transform.h"
#include "ResourceManager.h"

using namespace Minigin;

Text::Text(const std::string& text, Font* font) :
	m_NeedsUpdate{ true },
	m_Text{ text },
	m_Font{ font },
	m_Texture{ Renderer::Instance()->CreateTexture(font, text) }		
{

}

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

const std::string& Text::GetText() const
{
	return m_Text;
}

Font* Text::GetFont() const
{
	return m_Font.get();
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
