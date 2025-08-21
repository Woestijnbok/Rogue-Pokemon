#include <SDL.h>
#include <vector>
#include <utility>

#include "Mouse.h"
#include "Command.h"
#include "GameObject.h"

using namespace Minigin;

class Mouse::Impl final
{
public:
	explicit Impl() = default;
	~Impl() = default;

	Impl(const Impl& other) = delete;
	Impl(Impl&& other) noexcept = delete;
	Impl& operator=(const Impl& other) = delete;
	Impl& operator=(Impl&& other) noexcept = delete;

	bool ProcessInput();
	void AddInputAction(Action action, std::shared_ptr<Command> command);
	void ClearInputActions();
	void MouseMoved();
	void MouseClicked(bool left);
	void SetMousePosition(const glm::ivec2& position);
	void RemoveInputActions(GameObject* object);

private:
	std::vector<InputAction> m_InputActions;
	glm::ivec2 m_Position;
	bool m_Moved;
	std::pair<bool, bool> m_Clicks;
};

bool Mouse::Impl::ProcessInput()
{
	for (const InputAction& inputAction : m_InputActions)
	{
		switch (static_cast<Action>(inputAction.GetButton()))
		{
		case Action::LeftClick:
			if (m_Clicks.first)
			{
				inputAction.GetCommand()->Execute(m_Position);
				m_Clicks.first = false;
			}
			break;
		case Action::RightClick:
			if (m_Clicks.second)
			{
				inputAction.GetCommand()->Execute(m_Position);
				m_Clicks.second = false;
			}
			break;
		case Action::Move:
			if (m_Moved)
			{
				inputAction.GetCommand()->Execute(m_Position);		
				m_Moved = false;
			}
			break;
		}
	}

	return false;
}

void Mouse::Impl::AddInputAction(Action action, std::shared_ptr<Command> command)
{
	m_InputActions.emplace_back(static_cast<int>(action), InputAction::Trigger::None, command);
}

void Mouse::Impl::ClearInputActions()
{
	m_InputActions.clear();
}

void Mouse::Impl::MouseMoved()
{
	m_Moved = true;
}

void Mouse::Impl::MouseClicked(bool left)
{
	if (left) m_Clicks.first = true;
	else m_Clicks.second = true;
}

void Mouse::Impl::SetMousePosition(const glm::ivec2& position)
{
	m_Position = position;
}

void Mouse::Impl::RemoveInputActions(GameObject* object)		
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
}	

Mouse::Mouse() :
	m_Pimpl{ std::make_unique<Mouse::Impl>() }
{

}

Mouse::~Mouse() = default;

bool Minigin::Mouse::ProcessInput()
{
	return m_Pimpl->ProcessInput();
}

void Minigin::Mouse::AddInputAction(Action action, std::shared_ptr<Command> command)
{
	m_Pimpl->AddInputAction(action, command);
}

void Minigin::Mouse::ClearInputActions()
{
	m_Pimpl->ClearInputActions();
}

void Minigin::Mouse::MouseMoved()
{
	m_Pimpl->MouseMoved();
}

void Minigin::Mouse::MouseClicked(bool left)
{
	m_Pimpl->MouseClicked(left);
}

void Minigin::Mouse::SetMousePosition(const glm::ivec2& position)
{
	m_Pimpl->SetMousePosition(position);
}

void Minigin::Mouse::RemoveInputActions(GameObject* object)
{
	m_Pimpl->RemoveInputActions(object);
}