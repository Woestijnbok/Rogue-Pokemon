#pragma once

#include <string>
#include <memory>

#include "Component.h"

namespace Minigin
{
	class Font;
	class Text;	

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextComponent();

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) noexcept = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) noexcept = delete;

		virtual void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);
		std::shared_ptr<Font> GetFont();

	private:
		std::unique_ptr<Text> m_Text;

	};
}