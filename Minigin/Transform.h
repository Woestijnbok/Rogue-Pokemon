#pragma once

#include <vec2.hpp>	
#include <mat4x4.hpp>

namespace Minigin
{
	class Transform final
	{
	public:
		explicit Transform();
		explicit Transform(glm::ivec2 position, int rotation, glm::vec2 scale);	
		~Transform() = default;

		Transform(const Transform& rhs);
		Transform(Transform&& rhs) noexcept;
		Transform& operator=(const Transform& rhs);
		Transform& operator=(const Transform&& rhs) noexcept;

		void SetPosition(const glm::ivec2& position);
		void SetRotation(int rotation);	
		void SetScale(const glm::vec2& scale);
		const glm::ivec2& GetPosition() const;
		int GetRotation() const;		
		const glm::vec2& GetScale() const;
			
	private:
		glm::ivec2 m_Position;
		int m_Rotation;	
		glm::vec2 m_Scale;
	};
}