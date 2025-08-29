#include "Font.h"

#include <stdexcept>
#include <SDL_ttf.h>

#include "ResourceManager.h"

using namespace Minigin;

Font::Font(const std::filesystem::path& path, uint8_t size) :
	m_Font{ TTF_OpenFont(path.generic_string().c_str(), size) },
	m_Size{ size }
{
	if (m_Font == nullptr) throw std::runtime_error(std::string("Font::Font() - ") + SDL_GetError());	
}

Font::~Font()
{
	TTF_CloseFont(m_Font);
	ResourceManager::Instance()->RemoveFont(this);
}

TTF_Font* Font::GetFont() const
{
	return m_Font;
}

uint8_t Font::GetSize() const
{
	return m_Size;
}

FontKey::FontKey() :
	Name{ InvalidName },
	Size{ InvalidSize }
{

}

FontKey::FontKey(const std::string& name, uint8_t size) :
	Name{ name },
	Size{ size }
{

}

FontKey& Minigin::FontKey::operator=(const FontKey&& other)
{
	Name = other.Name;
	Size = other.Size;

	return *this;
}

bool Minigin::FontKey::IsValid() const
{
	return Name == InvalidName or Size == InvalidSize;
}

bool FontKey::operator==(const FontKey& other) const
{
	return Name == other.Name and Size == other.Size;
}