#pragma once

#include <string>
#include <memory>
#include <vec2.hpp>

#include "Font.h"
#include "Texture.h"
#include "Color.h"

namespace Minigin
{
	class Transform;

	class Text final
	{
	public:
		explicit Text(const std::string& text, Font* font, const Color& color);
		~Text() = default;

		Text(const Text& other) = delete;
		Text(Text&& other) noexcept = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) noexcept = delete;

		void SetText(const std::string& text);
		const std::string& GetText() const;
		Font* GetFont() const;
		Texture* GetTexture();

	private:
		void Update();

		bool m_NeedsUpdate;
		std::string m_Text;
		Font* m_Font;
		std::unique_ptr<Texture> m_Texture;
		Color m_Color;

	};
}