#pragma once

#include <filesystem>

struct _TTF_Font;

namespace Minigin
{
	class Font final
	{
	public:
		explicit Font(const std::filesystem::path& path, unsigned int size);
		~Font();

		Font(const Font&) = delete;
		Font(Font&&) noexcept = delete;
		Font& operator= (const Font&) = delete;
		Font& operator= (const Font&&) noexcept = delete;

		_TTF_Font* GetFont() const;

	private:
		_TTF_Font* const m_Font;

	};
}