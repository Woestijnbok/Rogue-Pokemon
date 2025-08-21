#pragma once

#include <vector>

#include "Singleton.h"
#include "Keyboard.h"
#include "Controller.h"
#include "Mouse.h"

namespace Minigin
{
	class GameObject;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		friend class Singleton<InputManager>;	

		~InputManager() = default;

		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) noexcept = delete;
		InputManager& operator= (const InputManager&) = delete;
		InputManager& operator= (const InputManager&&) noexcept = delete;

		bool ProcessInput();
		void AddController();
		Keyboard& GetKeyboard();
		Mouse& GetMouse();
		Controller& GetController(unsigned int index);
		void RemoveInputActions(GameObject* object);

	private:
		Mouse m_Mouse;
		Keyboard m_Keyboard;
		std::vector<Controller> m_Controllers;

		explicit InputManager();

	};
}