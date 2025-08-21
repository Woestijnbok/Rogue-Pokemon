#include "ImageComponent.h"
#include "Texture.h"
#include "Renderer.h"
#include "GameObject.h"	

using namespace Minigin;

ImageComponent::ImageComponent(GameObject* owner, std::shared_ptr<Texture> texture) :	
	Component{ owner },
	m_Texture{ texture }
{

}

void ImageComponent::Render() const
{
	m_Texture->Render(GetOwner()->GetWorldTransform());
}

const std::shared_ptr<Texture> ImageComponent::GetTexture() const
{
	return m_Texture;
}