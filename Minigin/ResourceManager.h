#pragma once

#include <memory>
#include <filesystem>

#include "Singleton.h"

namespace Minigin
{
	class Texture;
	class Font;
	class Sprite;		

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		friend class Singleton<ResourceManager>;

		~ResourceManager() = default;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;
		ResourceManager& operator= (const ResourceManager&) = delete;
		ResourceManager& operator= (const ResourceManager&&) noexcept = delete;

		std::shared_ptr<Texture> LoadTexture(const std::filesystem::path& path) const;
		std::shared_ptr<Font> LoadFont(const std::filesystem::path& path, unsigned int size) const;
		std::shared_ptr<Sprite> LoadSprite(const std::filesystem::path& path, int frames, int rows, int collumns);
		const std::filesystem::path& GetTextureRootPath() const;	
		const std::filesystem::path& GetFontRootPath() const;
		const std::filesystem::path& GetAudioRootPath() const;
		const std::filesystem::path& GetFileRootPath() const;

	private:
		const std::filesystem::path m_TextureRootDirectory;
		const std::filesystem::path m_FontRootDirectory;
		const std::filesystem::path m_AudioRootDirectory;
		const std::filesystem::path m_FileRootDirectory;

		explicit ResourceManager();

	};
}