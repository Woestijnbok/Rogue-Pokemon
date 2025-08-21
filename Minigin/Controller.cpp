#include <vector>
#include <windows.h>
#include <Xinput.h>
#include <iostream>
#include <vec2.hpp>

#include "Controller.h"
#include "Command.h"
#include "GameObject.h"

using namespace Minigin;

class Controller::Impl final
{
public:
	explicit Impl(unsigned int index);	
	~Impl() = default;		

	Impl(const Impl& other) = delete;
	Impl(Impl&& other) noexcept = default;
	Impl& operator=(const Impl& other) = delete;
	Impl& operator=(Impl&& other) noexcept = delete;

	void ProcessInput();
	void AddInputAction(Button button, InputAction::Trigger trigger, const std::shared_ptr<Command>& command);
	void ClearInputActions();
	unsigned int GetIndex() const;
	void RemoveInputActions(GameObject* object);

private:
	unsigned int m_Index;
	std::vector<InputAction> m_InputActions;
	std::vector<InputAction> m_JoystickActions;
	XINPUT_STATE m_PreviousState;
	XINPUT_STATE m_CurrentState;

	static const SHORT m_Deadzone;
	static const SHORT m_JoystickMaxValue;
	static const SHORT m_JoystickMinValue;

	unsigned int ConvertButton(Controller::Button button) const;
	float ProccessJoystick(SHORT value) const;
};

const SHORT Controller::Impl::m_Deadzone{ 10000 };
const SHORT Controller::Impl::m_JoystickMaxValue{ 32767 };
const SHORT Controller::Impl::m_JoystickMinValue{ -32768 };

Controller::Impl::Impl(unsigned int index) :
	m_Index{ index },
	m_InputActions{},
	m_JoystickActions{},
	m_PreviousState{},
	m_CurrentState{}
{

}

void Controller::Impl::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	if(XInputGetState(static_cast<DWORD>(m_Index), &m_CurrentState) != ERROR_SUCCESS) return;	
		
	WORD buttonChanges{ static_cast<WORD>(m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons) };		
	WORD buttonsPressedThisFrame{ static_cast<WORD>(buttonChanges & m_CurrentState.Gamepad.wButtons) };		
	WORD buttonsReleasedThisFrame{ static_cast<WORD>(buttonChanges & (~m_CurrentState.Gamepad.wButtons)) };			

	for (const InputAction& inputAction : m_InputActions)	
	{
		switch (inputAction.GetTrigger())		
		{
		case InputAction::Trigger::Down:
			if (m_CurrentState.Gamepad.wButtons & static_cast<WORD>(inputAction.GetButton()))	
			{
				inputAction.GetCommand()->Execute();	
			}
			break;
		case InputAction::Trigger::Pressed:
			if (buttonsPressedThisFrame & static_cast<WORD>(inputAction.GetButton()))	
			{
				inputAction.GetCommand()->Execute();			
			}
			break;
		case InputAction::Trigger::Up:	
			if (buttonsReleasedThisFrame & static_cast<WORD>(inputAction.GetButton()))	
			{
				inputAction.GetCommand()->Execute();		
			}
			break;
		}
	}

	if (m_JoystickActions.size() > 0)
	{
		float lx{ ProccessJoystick(m_CurrentState.Gamepad.sThumbLX) };
		float ly{ ProccessJoystick(m_CurrentState.Gamepad.sThumbLY) };
		float rx{ ProccessJoystick(m_CurrentState.Gamepad.sThumbRX) };
		float ry{ ProccessJoystick(m_CurrentState.Gamepad.sThumbRY) };

		const glm::vec2 left{ lx, ly };	
		const glm::vec2 right{ rx, ry };	

		for (const InputAction& inputAction : m_JoystickActions)
		{
			if (static_cast<Button>(inputAction.GetButton()) == Button::LeftJoystick)
			{
				inputAction.GetCommand()->Execute(left);
			}
			else
			{
				inputAction.GetCommand()->Execute(right);	
			}
		}
	}
}

void Controller::Impl::AddInputAction(Button button, InputAction::Trigger trigger, const std::shared_ptr<Command>& command)
{
	if (button == Button::LeftJoystick or button == Button::RightJoystick)
	{
		m_JoystickActions.emplace_back(static_cast<int>(button), trigger, command);
	}
	else 
	{
		m_InputActions.emplace_back(ConvertButton(button), trigger, command);
	}
}

void Controller::Impl::ClearInputActions()
{
	m_InputActions.clear();
}

unsigned int Controller::Impl::GetIndex() const
{
	return m_Index;
}

void Controller::Impl::RemoveInputActions(GameObject* object)	
{
	m_InputActions.erase
	(
		std::remove_if
		(
			m_InputActions.begin(), m_InputActions.end(), 
			[&object](const InputAction& inputAction) -> bool 
			{
				GameObjectCommand* command{ dynamic_cast<GameObjectCommand*>(inputAction.GetCommand()) };
				if (command != nullptr)
				{
					if (command->GetGameObject() == object) return true;
				}

				return false;
			}
		),
		m_InputActions.end()
	);

	m_JoystickActions.erase
	(
		std::remove_if
		(
			m_JoystickActions.begin(), m_JoystickActions.end(),
			[&object](const InputAction& inputAction) -> bool
			{
				GameObjectCommand* command{ dynamic_cast<GameObjectCommand*>(inputAction.GetCommand()) };
				if (command != nullptr)
				{
					if (command->GetGameObject() == object) return true;
				}

				return false;
			}
		),
		m_JoystickActions.end()
	);
}

unsigned int Controller::Impl::ConvertButton(Controller::Button button) const
{
	switch (button)
	{
	case Minigin::Controller::Button::Y:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_Y);
		break;
	case Minigin::Controller::Button::B:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_B);	
		break;
	case Minigin::Controller::Button::A:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_A);	
		break;
	case Minigin::Controller::Button::X:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_X);	
		break;	
	case Minigin::Controller::Button::DpadUp:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_DPAD_UP);		
		break;
	case Minigin::Controller::Button::DpadRight:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_DPAD_RIGHT);	
		break;
	case Minigin::Controller::Button::DpadDown:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_DPAD_DOWN);	
		break;
	case Minigin::Controller::Button::DpadLeft:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case Minigin::Controller::Button::RightShoulder:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_RIGHT_SHOULDER);	
		break;
	case Minigin::Controller::Button::RightTrigger:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_RIGHT_THUMB);
		break;
	case Minigin::Controller::Button::LeftShoulder:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_LEFT_SHOULDER);	
		break;
	case Minigin::Controller::Button::LeftTrigger:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_LEFT_THUMB);	
		break;
	case Minigin::Controller::Button::Start:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_START);
		break;
	case Minigin::Controller::Button::Back:
		return static_cast<unsigned int>(XINPUT_GAMEPAD_BACK);
		break;
	default:
		return 0U;
		break;
	}
}

float Controller::Impl::ProccessJoystick(SHORT value) const
{
	float newValue{};

	if (abs(value) < m_Deadzone)	
	{
		newValue = 0.0f;
	}
	else
	{
		if (value > 0)
		{
			newValue = float(value) / float(m_JoystickMaxValue);
		}
		else
		{
			newValue = -float(value) / float(m_JoystickMinValue);
		}
	}

	return newValue;
}

Controller::Controller(unsigned int index) :	
	m_Pimpl{ std::make_unique<Controller::Impl>(index) }
{

}

Controller::~Controller() = default;

Controller::Controller(Controller&& other) noexcept = default;

void Controller::ProcessInput()
{
	m_Pimpl->ProcessInput();
}

void Controller::AddInputAction(Button button, InputAction::Trigger trigger, const std::shared_ptr<Command>& command)
{
	m_Pimpl->AddInputAction(button, trigger, command);
}

void Controller::ClearInputActions()
{
	m_Pimpl->ClearInputActions();
}

unsigned int Controller::GetIndex() const
{
	return m_Pimpl->GetIndex();
}

void Minigin::Controller::RemoveInputActions(GameObject* object)
{
	m_Pimpl->RemoveInputActions(object);
}