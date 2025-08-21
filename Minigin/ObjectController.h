#pragma once

#include <concepts>
#include <type_traits>
#include <vector>
#include <functional>
#include <iterator>
#include <algorithm>

#include "ControllableObject.h"

namespace Minigin
{
	template<typename Type>
	concept Controllable = std::is_base_of_v<ControllableObject, Type>;

	template <typename Type>
	concept Updatable = requires(Type object)
	{
		{ object.FixedUpdate() } -> std::same_as<void>;
		{ object.Update() } -> std::same_as<void>;
		{ object.LateUpdate() } -> std::same_as<void>;
	};

	template <typename Type>
	concept Renderable = requires(const Type object)
	{
		{ object.Render() } -> std::same_as<void>;
	};

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	class ObjectController
	{
	public:
		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

	protected:
		explicit ObjectController() = default;
		~ObjectController();

		ObjectController(const ObjectController&) = delete;
		ObjectController(ObjectController&&) noexcept = delete;
		ObjectController& operator= (const ObjectController&) = delete;
		ObjectController& operator= (const ObjectController&&) noexcept = delete;

		void AddControllableObject(ObjectType* object);
		ObjectType* GetControllableObject(const std::function<bool(ObjectType*)>& predicate) const;
		void RemoveControllableObjects(const std::function<bool(ObjectType*)>& predicate);

	private:
		size_t m_InitialObjectCount;
		std::vector<ObjectType*> m_EnabledObjects;
		std::vector<ObjectType*> m_DisabledObjects;

		void CheckDestroyedObjects();
		void CheckDisabledObjects();
		void CheckEnabledObjects();

	};

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	ObjectController<ObjectType>::~ObjectController()
	{
		for (ObjectType*& object : m_EnabledObjects)
		{
			delete object;
			object = nullptr;
		}

		for (ObjectType*& object : m_DisabledObjects)
		{
			delete object;
			object = nullptr;
		}
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::FixedUpdate()
	{
		m_InitialObjectCount = m_EnabledObjects.size();

		for (size_t i{}; i < m_InitialObjectCount; ++i)
		{
			if (m_EnabledObjects.at(i)->GetStatus() == ControllableObject::Status::Enabled)
			{
				m_EnabledObjects.at(i)->FixedUpdate();
			}
		}

	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::Update()
	{
		m_InitialObjectCount = m_EnabledObjects.size();

		for (size_t i{}; i < m_InitialObjectCount; ++i)
		{
			if (m_EnabledObjects.at(i)->GetStatus() == ControllableObject::Status::Enabled)
			{
				m_EnabledObjects.at(i)->Update();
			}
		}
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::LateUpdate()
	{
		m_InitialObjectCount = m_EnabledObjects.size();

		for (size_t i{}; i < m_InitialObjectCount; ++i)
		{
			if (m_EnabledObjects.at(i)->GetStatus() == ControllableObject::Status::Enabled)
			{
				m_EnabledObjects.at(i)->LateUpdate();
			}
		}

		CheckDestroyedObjects();
		CheckDisabledObjects();
		CheckEnabledObjects();
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::Render() const
	{
		for (ObjectType* object : m_EnabledObjects)
		{
			object->Render();
		}
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::AddControllableObject(ObjectType* object)
	{
		if (object->GetStatus() == ControllableObject::Status::Enabled)
		{
			m_EnabledObjects.push_back(object);
		}
		else
		{
			m_DisabledObjects.push_back(object);
		}
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	ObjectType* ObjectController<ObjectType>::GetControllableObject(const std::function<bool(ObjectType*)>& predicate) const
	{
		auto enabledItertator
		{
			std::ranges::find_if
			(
				m_EnabledObjects,
				[&predicate](ObjectType* object) -> bool
				{
					return predicate(object);
				}
			)
		};

		if (enabledItertator != std::end(m_EnabledObjects))
		{
			return *enabledItertator;
		}
		else
		{
			auto disabledIterator
			{
				std::ranges::find_if
				(
					m_DisabledObjects,
					[&predicate](ObjectType* object) -> bool
					{
						return predicate(object);
					}
				)
			};

			if (disabledIterator != std::end(m_DisabledObjects))
			{
				return *disabledIterator;
			}
			else
			{
				return nullptr;
			}
		}
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::RemoveControllableObjects(const std::function<bool(ObjectType*)>& predicate)
	{
		std::ranges::for_each
		(
			m_EnabledObjects,
			[&predicate](ObjectType* object) -> void
			{
				if (predicate(object))
				{
					object->SetStatus(ControllableObject::Status::Destroyed);
				}
			}
		);

		std::ranges::for_each
		(
			m_DisabledObjects,
			[&predicate](ObjectType* object) -> void
			{
				if (predicate(object))
				{
					object->SetStatus(ControllableObject::Status::Destroyed);
				}
			}
		);
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::CheckDestroyedObjects()
	{
		m_EnabledObjects.erase
		(
			std::remove_if
			(
				std::begin(m_EnabledObjects), std::end(m_EnabledObjects),
				[](ObjectType* object) -> bool
				{
					if (object->GetStatus() == ControllableObject::Status::Destroyed)
					{
						delete object;
						return true;
					}
					else return false;
				}
			)
			, m_EnabledObjects.end()
		);

		m_DisabledObjects.erase
		(
			std::remove_if
			(
				std::begin(m_DisabledObjects), std::end(m_DisabledObjects),
				[](ObjectType* object) -> bool
				{
					if (object->GetStatus() == ControllableObject::Status::Destroyed)
					{
						delete object;
						return true;
					}
					else return false;
				}
			)
			, m_DisabledObjects.end()
		);
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::CheckDisabledObjects()
	{
		auto it = std::remove_if
		(
			std::begin(m_EnabledObjects),
			std::end(m_EnabledObjects),
			[this](ObjectType* object) -> bool
			{
				if (object->GetStatus() == ControllableObject::Status::Disabled)
				{
					m_DisabledObjects.push_back(object);
					return true;
				}
				else return false;
			}
		);

		if (it != m_EnabledObjects.end())
		{
			m_EnabledObjects.erase(it, std::end(m_EnabledObjects));
		}
	}

	template<typename ObjectType>
		requires Controllable<ObjectType>&& Updatable<ObjectType>&& Renderable<ObjectType>
	void ObjectController<ObjectType>::CheckEnabledObjects()
	{
		auto it = std::remove_if
		(
			std::begin(m_DisabledObjects),
			std::end(m_DisabledObjects),
			[this](ObjectType* object) -> bool
			{
				if (object->GetStatus() == ControllableObject::Status::Enabled)
				{
					m_EnabledObjects.push_back(object);
					return true;
				}
				return false;
			}
		);

		if (it != m_DisabledObjects.end())
		{
			m_DisabledObjects.erase(it, std::end(m_DisabledObjects));
		}
	}
}