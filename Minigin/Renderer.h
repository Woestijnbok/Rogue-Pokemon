#pragma once

#include <memory>
#include <filesystem>
#include <vec2.hpp>

#include "Singleton.h"

namespace Minigin
{
	class Texture;					
	class Font;
	class Transform;
	class Sprite;

	class Renderer final : public Singleton<Renderer>
	{
	public:
		friend class Singleton<Renderer>;

		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator= (const Renderer&) = delete;
		Renderer& operator= (const Renderer&&) noexcept = delete;

		void Render() const;
		Texture* CreateTexture(const std::filesystem::path& path) const;
		Texture* CreateTexture(Font* font, const std::string& text) const;
		/*
		* @brief renders a texture centered around the give transform.
		*
		* @param The scale makes the texture possibly strectch or shrink and flip with negative values.
		* The position will be the center point of the texture.
		* The rotation will dictate the rotation of the texture around the center of itself.
		* The center is the point at wich the texture will rotate around (point in texture space).	
		*/
		void RenderTexture(const Texture& texture, const Transform& transform, const glm::ivec2& center = glm::ivec2{ -1 }) const;
		/*
		* @brief renders a certain frame of a sprite centered around the give transform.
		*
		* @param The scale makes the frame possibly strectch or shrink and flip with negative values.
		* The position will be the center point of the frame.
		* The rotation will dictate the rotation of the frame around the center of itself.
		*/
		void RenderSprite(const Sprite& sprite, int frame, const Transform& transform) const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_Pimpl;	

		explicit Renderer();

	};
}