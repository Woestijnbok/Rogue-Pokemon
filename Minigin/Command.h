#pragma once

#include <vec2.hpp>

namespace Minigin
{
	class GameObject;

	class Command
	{
	public:
		explicit Command() = default;
		virtual ~Command() = default;

		Command(const Command&) = delete;
		Command(Command&&) noexcept = delete;
		Command& operator= (const Command&) = delete;
		Command& operator= (const Command&&) noexcept = delete;

		virtual void Execute();
		virtual void Execute(const glm::vec2& axis);

	protected:


	private:


	};

	class GameObjectCommand : public Command
	{
	public:
		explicit GameObjectCommand(GameObject* gameObject);
		virtual ~GameObjectCommand() = default;

		GameObjectCommand(const GameObjectCommand&) = delete;
		GameObjectCommand(GameObjectCommand&&) noexcept = delete;
		GameObjectCommand& operator= (const GameObjectCommand&) = delete;
		GameObjectCommand& operator= (const GameObjectCommand&&) noexcept = delete;

		bool IsInvalid() const;
		GameObject* GetGameObject() const;

	protected:


	private:
		GameObject* m_GameObject;

	};
}