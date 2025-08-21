#include "InputAction.h"
#include "Command.h"
#include "GameObject.h"

using namespace Minigin;

InputAction::InputAction(unsigned int button, InputAction::Trigger trigger, const std::shared_ptr<Command>& command) :
	m_Button{ button },			
	m_Trigger{ trigger },
	m_Command{ command }
{
	
}

InputAction& Minigin::InputAction::operator=(const InputAction& rhs)
{
	m_Button = rhs.m_Button;
	m_Trigger = rhs.m_Trigger;
	m_Command = rhs.m_Command;

	return *this;
}

InputAction& Minigin::InputAction::operator=(const InputAction&& rhs) noexcept
{
	m_Button = rhs.m_Button;
	m_Trigger = rhs.m_Trigger;
	m_Command = rhs.m_Command;

	return *this;
}

unsigned int InputAction::GetButton() const
{
	return m_Button;
}

InputAction::Trigger InputAction::GetTrigger() const
{
	return m_Trigger;
}

Command* InputAction::GetCommand() const
{
	return m_Command.get();	
}
