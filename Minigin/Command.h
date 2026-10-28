#pragma once
#include <vec2.hpp>
#include "Component.h"
#include "GameObject.h"

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

		virtual bool IsValid() const;
		GameObject* GetGameObject() const;

	protected:


	private:
		GameObject* m_GameObject;

	};

	template<typename ComponentType>
		requires IsComponent<ComponentType>
	class ComponentCommand : public Minigin::GameObjectCommand
	{
	public:
		explicit ComponentCommand(ComponentType* component) :
			GameObjectCommand(component->GetOwner()),
			m_Component{ component }
		{

		}

		virtual ~ComponentCommand() = default;

		ComponentCommand(const ComponentCommand&) = delete;
		ComponentCommand(ComponentCommand&&) noexcept = delete;
		ComponentCommand& operator= (const ComponentCommand&) = delete;
		ComponentCommand& operator= (const ComponentCommand&&) noexcept = delete;

		virtual bool IsValid() const override
		{
			return GameObjectCommand::IsValid() and m_Component->GetStatus() == ControllableObject::Status::Enabled;
		}

		ComponentType* GetComponent() const
		{
			return m_Component;
		}
	private:
		ComponentType* m_Component;

	};
}