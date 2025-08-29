#include <stdexcept>
#include <SDL_ttf.h>
#include <algorithm>
#include <ranges>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"

using namespace Minigin;

ResourceManager::ResourceManager() :	
	Singleton{},
	m_TextureRootDirectory{ "Resources/Textures" },
	m_FontRootDirectory{ "Resources/Fonts" },
	m_AudioRootDirectory{ "Resources/Audio" },
	m_FileRootDirectory{ "Resources/Files" },
	m_Fonts{}
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

Font* ResourceManager::LoadFont(const std::filesystem::path& path, const std::string& name, uint8_t size)
{
	const FontKey fontKey{ name, size };

	assert(!m_Fonts.contains(fontKey));

	const std::filesystem::path fullPath{ m_FontRootDirectory / path };	

	if (std::filesystem::exists(fullPath))
	{
		if (!std::filesystem::is_regular_file(fullPath))
		{
			throw std::runtime_error("ResourceManager::CreateFont() - path given isn't a regular file");
		}
	}
	else throw std::runtime_error("ResourceManager::CreateFont() - path given doesn't exist");

	Font* font{ new Font{ fullPath, size } };
	if (!m_Fonts.emplace(fontKey, font).second)
	{
		throw std::runtime_error("ResourceManager::CreateFont() - Couldn't emplace font in fonts collection");
	}

	return font;
}

Font* Minigin::ResourceManager::GetOrLoadFont(const std::filesystem::path& path, const std::string& name, uint8_t size)
{
	const FontKey fontKey{ name, size };
	Font* font{ nullptr };

	if (m_Fonts.contains(fontKey))
	{
		font = m_Fonts.find(fontKey)->second;
	}
	else
	{
		font = LoadFont(path, name, size);
	}

	return font;
}

void ResourceManager::RemoveFont(Font* font)
{
	const auto iterator{ std::ranges::find_if(m_Fonts, [font](const std::pair<const FontKey, Font*>& pair) -> bool { return pair.second == font; }) };

	if (iterator == m_Fonts.end())
	{
		throw std::runtime_error("ResourceManager::RemoveFont() - Couldn't find the font to remove");
	}

	m_Fonts.erase(iterator->first);
}

Font* ResourceManager::GetFont(const std::string& name, uint8_t size) const
{
	const FontKey fontKey{ name, size };
	Font* font{ m_Fonts.find(fontKey)->second };

	if (font == nullptr)
	{
		throw std::runtime_error("ResourceManager::GetFont() - Couldn't find the font");
	}

	return font;
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