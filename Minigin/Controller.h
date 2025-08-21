#pragma once

#include <memory>

#include "InputAction.h"

namespace Minigin	
{
	class GameObject;

	class Controller final
	{
	public:
		enum class Button : unsigned int
		{
			Y,
			B,
			A,
			X,
			DpadUp,
			DpadRight,
			DpadDown,
			DpadLeft,
			RightShoulder,
			RightTrigger,
			LeftShoulder,
			LeftTrigger,
			Start,
			Back,
			LeftJoystick,
			RightJoystick
		};

		explicit Controller(unsigned int index);
		~Controller();

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) noexcept;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) noexcept = delete;

		void ProcessInput();
		void AddInputAction(Button button, InputAction::Trigger trigger, const std::shared_ptr<Command>& command);
		void ClearInputActions();
		unsigned int GetIndex() const;
		void RemoveInputActions(GameObject* object);

	private:
		class Impl;
		std::unique_ptr<Controller::Impl> m_Pimpl;

	};
}