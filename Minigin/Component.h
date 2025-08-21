#pragma once

#include "ControllableObject.h"

namespace Minigin
{
	class GameObject;

	class Component : public ControllableObject
	{
	public:
		explicit Component(GameObject* owner);
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = delete;

		GameObject* GetOwner() const;

		virtual void Update();
		virtual void FixedUpdate();
		virtual void LateUpdate();
		virtual void Render() const;

	private:
		GameObject* const m_Owner;

	};
}