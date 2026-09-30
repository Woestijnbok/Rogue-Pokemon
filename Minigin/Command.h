#pragma once

#include <vec2.hpp>

namespace Minigin
{
	class GameObject;
	class Component;

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

		virtual bool IsValid() const;
		GameObject* GetGameObject() const;

	protected:


	private:
		GameObject* m_GameObject;

	};

	class ComponentCommand : public Minigin::GameObjectCommand
	{
	public:
		explicit ComponentCommand(Component* component);
		virtual ~ComponentCommand() = default;

		ComponentCommand(const ComponentCommand&) = delete;
		ComponentCommand(ComponentCommand&&) noexcept = delete;
		ComponentCommand& operator= (const ComponentCommand&) = delete;
		ComponentCommand& operator= (const ComponentCommand&&) noexcept = delete;

		virtual bool IsValid() const override;
		Component* GetComponent() const;

	protected:


	private:
		Component* m_Component;

	};
}