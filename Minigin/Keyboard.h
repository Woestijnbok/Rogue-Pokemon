#pragma once

#include <memory>

#include "InputAction.h"

namespace Minigin
{
	class Mouse;
	class GameObject;

	class Keyboard final
	{
	public:
		enum class Key : unsigned int
		{
			One,
			Two,
			Three,
			Four,
			Five,
			Six,
			Seven,
			Eight,
			Nine,
			Zero,
			Q,
			W,
			E,
			R,
			T,
			Y,
			U,
			I,
			O,
			P,
			A,
			S,
			D,
			F,
			G,
			H,
			J,
			K,
			L,
			Z,
			X,
			C,
			V,
			B,
			N,
			M,
			Up,
			Right,
			Down,
			Left,
			Backspace,
			F1
		};

		explicit Keyboard();
		~Keyboard();

		Keyboard(const Keyboard& other) = delete;
		Keyboard(Keyboard&& other) noexcept = delete;
		Keyboard& operator=(const Keyboard& other) = delete;
		Keyboard& operator=(Keyboard&& other) noexcept = delete;

		bool ProcessInput();
		void AddInputAction(Key key, InputAction::Trigger trigger, const std::shared_ptr<Command>& command);
		void ClearInputActions();
		void SetMouse(Mouse* mouse);
		void RemoveInputActions(GameObject* object);

	private:
		class Impl;
		std::unique_ptr<Keyboard::Impl> m_Pimpl;

	};
}