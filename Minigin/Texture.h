#pragma once

#include <vec2.hpp>

struct SDL_Texture;

namespace Minigin
{
	class Transform;

	class Texture final
	{
	public:
		explicit Texture(SDL_Texture* texture);	
		~Texture();

		Texture(const Texture&) = delete;
		Texture(Texture&&) noexcept = delete;
		Texture& operator= (const Texture&) = delete;
		Texture& operator= (const Texture&&) noexcept = delete;

		/*
		* @brief renders self centered around the give transform.
		*
		* @param The scale makes the texture possibly strectch or shrink and flip with negative values.
		* The position will be the center point of the texture.
		* The rotation will dictate the rotation of the texture around the center of itself.
		* The center is the point at wich the texture will rotate around (point in texture space).
		*/
		void Render(const Transform& transform, const glm::ivec2& center = glm::ivec2{ -1 }) const;
		SDL_Texture* GetTexture() const;	
		glm::ivec2 GetSize() const;

	private:
		SDL_Texture* const m_Texture;

	};
}