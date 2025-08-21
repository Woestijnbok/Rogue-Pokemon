#pragma once

#include <memory>
#include <vec2.hpp>

namespace Minigin
{
	class Texture;

	class Sprite final
	{
	public:
		explicit Sprite(Texture* sheet, int frames, int rows, int collumns);
		~Sprite() = default;

		Sprite(const Sprite&) = delete;
		Sprite(Sprite&&) noexcept = delete;
		Sprite& operator= (const Sprite&) = delete;
		Sprite& operator= (const Sprite&&) noexcept = delete;

		Texture* GetSheet() const;
		int GetFrames() const;
		int GetRows() const;
		int GetCollumns() const;
		glm::ivec2 GetFrameSize() const;

	private:
		std::unique_ptr<Texture> m_Sheet;
		const int m_Frames;
		const int m_Rows;
		const int m_Collumns;

	};
}