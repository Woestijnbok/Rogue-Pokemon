#include <vector>
#include <SDL_events.h>

#include "backends/imgui_impl_sdl2.h"
#include "Keyboard.h"
#include "Command.h"
#include "InputManager.h"
#include "Engine.h"
#include "GameObject.h"

using namespace Minigin;

class Keyboard::Impl final
{
public:
	explicit Impl() = default;
	~Impl() = default;

	Impl(const Impl& other) = delete;
	Impl(Impl&& other) noexcept = delete;
	Impl& operator=(const Impl& other) = delete;
	Impl& operator=(Impl&& other) noexcept = delete;

	bool ProcessInput();
	void AddInputAction(Key key, InputAction::Trigger trigger, const std::shared_ptr<Command>& command);
	void ClearInputActions();
	void SetMouse(Mouse* mouse);
	void RemoveInputActions(GameObject* object);	

private:
	std::vector<InputAction> m_InputActions;
	Mouse* m_Mouse;

	unsigned int ConvertKey(Keyboard::Key key) const;

};

bool Keyboard::Impl::ProcessInput()
{
	SDL_Event event{};

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return true;
			break;
		case SDL_MOUSEMOTION:
			m_Mouse->MouseMoved();
			m_Mouse->SetMousePosition(glm::ivec2{ static_cast<int>(event.motion.x), Engine::GetWindowSize().y - static_cast<int>(event.motion.y) });	
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_Mouse->SetMousePosition(glm::ivec2{ static_cast<int>(event.button.x), Engine::GetWindowSize().y - static_cast<int>(event.button.y) });
			if (event.button.button == SDL_BUTTON_LEFT)	m_Mouse->MouseClicked(true);
			else m_Mouse->MouseClicked(false);
			break;
		case SDL_KEYDOWN:	
			if (event.key.keysym.sym == SDLK_ESCAPE)	
			{
				return true;
			}

			for (const InputAction& inputAction : m_InputActions)	
			{
				if ((event.key.keysym.sym == static_cast<SDL_KeyCode>(inputAction.GetButton())) and	
					(inputAction.GetTrigger() == InputAction::Trigger::Pressed))	
				{
					inputAction.GetCommand()->Execute();	
				}
			}
			break;
		case SDL_KEYUP:
			for (const InputAction& inputAction : m_InputActions)	
			{
				if ((event.key.keysym.sym == static_cast<SDL_KeyCode>(inputAction.GetButton())) and	
					(inputAction.GetTrigger() == InputAction::Trigger::Up))	
				{
					inputAction.GetCommand()->Execute();	
				}
			}
			break;
		default:
			break;
		}

		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);	
	for (const InputAction& inputAction : m_InputActions)	
	{
		if ((keyboardState[SDL_GetScancodeFromKey(static_cast<SDL_KeyCode>(inputAction.GetButton()))]) and	
			(inputAction.GetTrigger() == InputAction::Trigger::Down))	
		{	
			inputAction.GetCommand()->Execute();	
		}
	}

	return false;
}

void Keyboard::Impl::AddInputAction(Key key, InputAction::Trigger trigger, const std::shared_ptr<Command>& command)
{
	m_InputActions.emplace_back(ConvertKey(key), trigger, command);
}

void Keyboard::Impl::ClearInputActions()
{
	m_InputActions.clear();
}

void Keyboard::Impl::SetMouse(Mouse* mouse)
{
	m_Mouse = mouse;
}

void Keyboard::Impl::RemoveInputActions(GameObject* object)	
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

unsigned int Keyboard::Impl::ConvertKey(Keyboard::Key key) const
{
	switch (key)
	{
	case Minigin::Keyboard::Key::One:
		return static_cast<unsigned int>(SDLK_1);
		break;
	case Minigin::Keyboard::Key::Two:
		return static_cast<unsigned int>(SDLK_2);
		break;
	case Minigin::Keyboard::Key::Three:
		return static_cast<unsigned int>(SDLK_3);
		break;
	case Minigin::Keyboard::Key::Four:
		return static_cast<unsigned int>(SDLK_4);
		break;
	case Minigin::Keyboard::Key::Five:
		return static_cast<unsigned int>(SDLK_5);
		break;
	case Minigin::Keyboard::Key::Six:
		return static_cast<unsigned int>(SDLK_6);
		break;
	case Minigin::Keyboard::Key::Seven:
		return static_cast<unsigned int>(SDLK_7);
		break;
	case Minigin::Keyboard::Key::Eight:
		return static_cast<unsigned int>(SDLK_8);
		break;
	case Minigin::Keyboard::Key::Nine:
		return static_cast<unsigned int>(SDLK_9);
		break;
	case Minigin::Keyboard::Key::Zero:
		return static_cast<unsigned int>(SDLK_0);
		break;
	case Minigin::Keyboard::Key::Q:
		return static_cast<unsigned int>(SDLK_q);
		break;
	case Minigin::Keyboard::Key::W:
		return static_cast<unsigned int>(SDLK_w);
		break;
	case Minigin::Keyboard::Key::E:
		return static_cast<unsigned int>(SDLK_e);
		break;
	case Minigin::Keyboard::Key::R:
		return static_cast<unsigned int>(SDLK_r);
		break;
	case Minigin::Keyboard::Key::T:
		return static_cast<unsigned int>(SDLK_t);
		break;
	case Minigin::Keyboard::Key::Y:
		return static_cast<unsigned int>(SDLK_y);
		break;
	case Minigin::Keyboard::Key::U:
		return static_cast<unsigned int>(SDLK_u);
		break;
	case Minigin::Keyboard::Key::I:
		return static_cast<unsigned int>(SDLK_i);
		break;
	case Minigin::Keyboard::Key::O:
		return static_cast<unsigned int>(SDLK_o);
		break;
	case Minigin::Keyboard::Key::P:
		return static_cast<unsigned int>(SDLK_p);
		break;
	case Minigin::Keyboard::Key::A:
		return static_cast<unsigned int>(SDLK_a);
		break;
	case Minigin::Keyboard::Key::S:
		return static_cast<unsigned int>(SDLK_s);
		break;
	case Minigin::Keyboard::Key::D:
		return static_cast<unsigned int>(SDLK_d);
		break;
	case Minigin::Keyboard::Key::F:
		return static_cast<unsigned int>(SDLK_f);
		break;
	case Minigin::Keyboard::Key::G:
		return static_cast<unsigned int>(SDLK_g);
		break;
	case Minigin::Keyboard::Key::H:
		return static_cast<unsigned int>(SDLK_h);
		break;
	case Minigin::Keyboard::Key::J:
		return static_cast<unsigned int>(SDLK_j);
		break;
	case Minigin::Keyboard::Key::K:
		return static_cast<unsigned int>(SDLK_k);
		break;
	case Minigin::Keyboard::Key::L:
		return static_cast<unsigned int>(SDLK_l);
		break;
	case Minigin::Keyboard::Key::Z:
		return static_cast<unsigned int>(SDLK_z);
		break;
	case Minigin::Keyboard::Key::X:
		return static_cast<unsigned int>(SDLK_x);
		break;
	case Minigin::Keyboard::Key::C:
		return static_cast<unsigned int>(SDLK_c);
		break;
	case Minigin::Keyboard::Key::V:
		return static_cast<unsigned int>(SDLK_v);
		break;
	case Minigin::Keyboard::Key::B:
		return static_cast<unsigned int>(SDLK_b);	
		break;
	case Minigin::Keyboard::Key::N:
		return static_cast<unsigned int>(SDLK_n);	
		break;
	case Minigin::Keyboard::Key::M:
		return static_cast<unsigned int>(SDLK_m);	
		break;
	case Minigin::Keyboard::Key::Up:
		return static_cast<unsigned int>(SDLK_UP);	
		break;
	case Minigin::Keyboard::Key::Right:
		return static_cast<unsigned int>(SDLK_RIGHT);	
		break;
	case Minigin::Keyboard::Key::Down:
		return static_cast<unsigned int>(SDLK_DOWN);	
		break;
	case Minigin::Keyboard::Key::Left:
		return static_cast<unsigned int>(SDLK_LEFT);	
		break;
	case Minigin::Keyboard::Key::Backspace:
		return static_cast<unsigned int>(SDLK_BACKSPACE);	
		break;
	case Minigin::Keyboard::Key::F1:
		return static_cast<unsigned int>(SDLK_F1);	
		break;
	default:
		return 0U;
		break;
	}
}

Keyboard::Keyboard() :
	m_Pimpl{ std::make_unique<Keyboard::Impl>() }
{

}

Keyboard::~Keyboard() = default;

bool Keyboard::ProcessInput()
{
	return m_Pimpl->ProcessInput();
}

void Keyboard::AddInputAction(Key key, InputAction::Trigger trigger, const std::shared_ptr<Command>& command)
{
	m_Pimpl->AddInputAction(key, trigger, command);
}

void Keyboard::ClearInputActions()
{
	m_Pimpl->ClearInputActions();
}

void Minigin::Keyboard::SetMouse(Mouse* mouse)
{
	m_Pimpl->SetMouse(mouse);
}

void Minigin::Keyboard::RemoveInputActions(GameObject* object)
{
	m_Pimpl->RemoveInputActions(object);
}