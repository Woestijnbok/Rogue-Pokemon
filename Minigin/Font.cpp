#include <stdexcept>
#include <SDL_ttf.h>

#include "Font.h"

using namespace Minigin;

Font::Font(const std::filesystem::path& path, unsigned int size) : 
	m_Font{ TTF_OpenFont(path.generic_string().c_str(), size) }	
{
	if (m_Font == nullptr) throw std::runtime_error(std::string("Font::Font() - ") + SDL_GetError());	
}

Font::~Font()
{
	TTF_CloseFont(m_Font);
}

TTF_Font* Font::GetFont() const
{
	return m_Font;
}