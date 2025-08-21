#include <SDL_image.h>

#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"

using namespace Minigin;

Texture::Texture(SDL_Texture* texture) :
	m_Texture{ texture }
{
	
}

Texture::~Texture()	
{
	SDL_DestroyTexture(m_Texture);
}

void Texture::Render(const Transform& transform, const glm::ivec2& center) const
{
	Renderer::Instance()->RenderTexture(*this, transform, center);
}

SDL_Texture* Texture::GetTexture() const
{
	return m_Texture;
}

glm::ivec2 Texture::GetSize() const
{
	int width{}, height;
	SDL_QueryTexture(m_Texture, nullptr, nullptr, &width, &height);	
	return glm::ivec2{ width, height };
}