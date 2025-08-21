#pragma once

#include <memory>
#include <vec2.hpp>

#include "InputAction.h"

namespace Minigin
{
	class GameObject;
	
	class Mouse final
	{
	public:
		enum class Action : unsigned int
		{
			LeftClick,
			RightClick,
			Move
		};

		explicit Mouse();
		~Mouse();

		Mouse(const Mouse& other) = delete;
		Mouse(Mouse&& other) noexcept = delete;
		Mouse& operator=(const Mouse& other) = delete;
		Mouse& operator=(Mouse&& other) noexcept = delete;

		bool ProcessInput();
		void AddInputAction(Action action, std::shared_ptr<Command> command);
		void ClearInputActions();
		void MouseMoved();
		void MouseClicked(bool left);
		void SetMousePosition(const glm::ivec2& position);
		void RemoveInputActions(GameObject* object);	

	private:
		class Impl;
		std::unique_ptr<Mouse::Impl> m_Pimpl;
	};
}