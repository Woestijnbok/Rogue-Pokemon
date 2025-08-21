#pragma once

#include <string>
#include <memory>
#include <vec2.hpp>

namespace Minigin
{
	class Font;
	class Texture;
	class Transform;

	class Text final
	{
	public:
		explicit Text(const std::string& text, std::shared_ptr<Font> font);
		virtual ~Text();

		Text(const Text& other) = delete;
		Text(Text&& other) noexcept = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) noexcept = delete;

		void Update();
		/*
		* @brief renders self centered around the give transform.
		*
		* @param The scale makes the text possibly strectch or shrink and flip with negative values.
		* The position will be the center point of the text.
		* The rotation will dictate the rotation of the text around the center of itself.
		*/
		void Render(const Transform& transform) const;	
		void SetText(const std::string& text);
		std::shared_ptr<Font> GetFont() const;
		glm::ivec2 GetSize();

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::unique_ptr<Texture> m_Texture;

	};
}