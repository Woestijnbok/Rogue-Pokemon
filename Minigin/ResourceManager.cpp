#include <stdexcept>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include "Sprite.h"

using namespace Minigin;

ResourceManager::ResourceManager() :	
	Singleton{},	
	m_TextureRootDirectory{ "Resources/Textures" },	
	m_FontRootDirectory{ "Resources/Fonts" },
	m_AudioRootDirectory{ "Resources/Audio" },
	m_FileRootDirectory{ "Resources/Files" }
{	
	// Checking texture root directory	
	if (std::filesystem::exists(m_TextureRootDirectory))
	{
		if (!std::filesystem::is_directory(m_TextureRootDirectory))
		{
			throw std::runtime_error("ResourceManager::ResourceManager() - texture root directory isn't a directory");
		}
	}
	else throw std::runtime_error("ResourceManager::ResourceManager() - texture root directory doesn't exist");

	// Checking font root directory
	if (std::filesystem::exists(m_FontRootDirectory))
	{
		if (!std::filesystem::is_directory(m_FontRootDirectory))
		{
			throw std::runtime_error("ResourceManager::ResourceManager() - font root directory isn't a directory");
		}
	}
	else throw std::runtime_error("ResourceManager::ResourceManager() - font root directory doesn't exist");

	// Checking audio root directory
	if (std::filesystem::exists(m_AudioRootDirectory))
	{
		if (!std::filesystem::is_directory(m_AudioRootDirectory))
		{
			throw std::runtime_error("ResourceManager::ResourceManager() - audio root directory isn't a directory");
		}
	}
	else throw std::runtime_error("ResourceManager::ResourceManager() - audio root directory doesn't exist");

	// Checking file root directory
	if (std::filesystem::exists(m_FileRootDirectory))
	{
		if (!std::filesystem::is_directory(m_FileRootDirectory))
		{
			throw std::runtime_error("ResourceManager::ResourceManager() - file root directory isn't a directory");
		}
	}
	else throw std::runtime_error("ResourceManager::ResourceManager() - file root directory doesn't exist");
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::filesystem::path& path) const
{
	const std::filesystem::path fullPath{ m_TextureRootDirectory / path };

	if (std::filesystem::exists(fullPath))
	{
		if (!std::filesystem::is_regular_file(fullPath))
		{
			throw std::runtime_error("ResourceManager::CreateTexture() - path given isn't a regular file");
		}
	}
	else throw std::runtime_error("ResourceManager::CreateTexture() - path given doesn't exist");

	return std::shared_ptr<Texture>(Renderer::Instance()->CreateTexture(fullPath));		
}

std::shared_ptr<Font> ResourceManager::LoadFont(const std::filesystem::path& path, unsigned int size) const	
{
	const std::filesystem::path fullPath{ m_FontRootDirectory / path };	

	if (std::filesystem::exists(fullPath))
	{
		if (!std::filesystem::is_regular_file(fullPath))
		{
			throw std::runtime_error("ResourceManager::CreateFont() - path given isn't a regular file");
		}
	}
	else throw std::runtime_error("ResourceManager::CreateFont() - path given doesn't exist");

	return std::make_shared<Font>(fullPath, size);	
}

std::shared_ptr<Sprite> Minigin::ResourceManager::LoadSprite(const std::filesystem::path& path, int frames, int rows, int collumns)
{
	const std::filesystem::path fullPath{ m_TextureRootDirectory / path };	

	if (std::filesystem::exists(fullPath))	
	{
		if (!std::filesystem::is_regular_file(fullPath))	
		{
			throw std::runtime_error("ResourceManager::CreateFont() - path given isn't a regular file");	
		}
	}
	else throw std::runtime_error("ResourceManager::CreateFont() - path given doesn't exist");	

	return std::make_shared<Sprite>(Renderer::Instance()->CreateTexture(fullPath), frames, rows, collumns);
}

const std::filesystem::path& ResourceManager::GetTextureRootPath() const
{
	return m_TextureRootDirectory;
}

const std::filesystem::path& ResourceManager::GetFontRootPath() const
{
	return m_FontRootDirectory;
}

const std::filesystem::path& ResourceManager::GetAudioRootPath() const
{
	return m_AudioRootDirectory;
}

const std::filesystem::path& Minigin::ResourceManager::GetFileRootPath() const
{
	return m_FileRootDirectory;
}