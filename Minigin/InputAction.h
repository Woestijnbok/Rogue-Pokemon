#pragma once

#include <optional>
#include <memory>
#include <vec2.hpp>

namespace Minigin
{
	class Command;

	class InputAction final
	{
	public:
		enum class Trigger : unsigned int
		{
			Up,			// When key is released this frame.
			Pressed,	// When key is pressed this frame.
			Down,       // When key is down.
			None		// Some require none or only have one.
		};

		/*
		*  @param axis wether or not this command has axis input like mouse or joysticks.
		*/
		explicit InputAction(unsigned int button, InputAction::Trigger trigger, const std::shared_ptr<Command>& command);
		~InputAction() = default;

		InputAction(const InputAction& rhs) = default;
		InputAction(InputAction&& rhs) noexcept = default;
		InputAction& operator= (const InputAction& rhs);
		InputAction& operator= (const InputAction&& rhs) noexcept;
		
		unsigned int GetButton() const;
		Trigger GetTrigger() const;
		Command* GetCommand() const;

	private:
		unsigned int m_Button;
		InputAction::Trigger m_Trigger;
		std::shared_ptr<Command> m_Command;
	};
}