#pragma once

#include <string>
#include <memory>

#include "Component.h"
#include "Color.h"

namespace Minigin
{
	class Font;
	class Text;	

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(GameObject* owner, const std::string& text, Font* font, const Color& color);
		virtual ~TextComponent();

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) noexcept = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) noexcept = delete;

		virtual void Render() const override;

		void SetText(const std::string& text);
		Font* GetFont();

	private:
		std::unique_ptr<Text> m_Text;

	};
}