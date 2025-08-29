#pragma once

#include <filesystem>
#include <string>
#include <cstdint>

struct _TTF_Font;

namespace Minigin
{
	class Font final
	{
	public:
		explicit Font(const std::filesystem::path& path, uint8_t size);
		~Font();

		Font(const Font&) = delete;
		Font(Font&&) noexcept = delete;
		Font& operator= (const Font&) = delete;
		Font& operator= (const Font&&) noexcept = delete;

		_TTF_Font* GetFont() const;
		uint8_t GetSize() const;

	private:
		_TTF_Font* const m_Font;
		uint8_t m_Size;

	};

	struct FontKey final
	{
		explicit FontKey();
		explicit FontKey(const std::string& name, uint8_t size);
		~FontKey() = default;

		FontKey(const FontKey& other) = default;
		FontKey(FontKey&& other) = default;
		FontKey& operator= (const FontKey& other) = default;
		FontKey& operator= (const FontKey&& other);

		bool IsValid() const;

		bool operator==(const FontKey& other) const;

		std::string Name;
		uint8_t Size;

		static constexpr char InvalidName[5]{ "null" };		// std::string only has constexpr constructor from c++ 23 onwards
		static constexpr uint8_t InvalidSize{ 0 };
	};
}

namespace std 
{
	template <>
	struct hash<Minigin::FontKey>
	{
		std::size_t operator()(const Minigin::FontKey& fontKey) const
		{
			std::size_t hashName{ std::hash<std::string>{}(fontKey.Name) };
			std::size_t hashSize{ std::hash<uint8_t>{}(fontKey.Size) };

			return std::size_t{ hashName ^ (hashSize + 0x9e3779b9 + (hashName << 6) + (hashName >> 2)) };
		}
	};
}