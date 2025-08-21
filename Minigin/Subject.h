#pragma once

#include <vector>
#include <functional>
#include <algorithm>

namespace Minigin
{
	template<typename... Arguments>
	class Subject final
	{
	public:
		explicit Subject() = default;
		~Subject() = default;

		Subject(const Subject& other) = delete;
		Subject(Subject&& other) noexcept = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) noexcept = delete;

		void AddObserver(const std::function<void(Arguments...)>& observer);
		void RemoveObserver(const std::function<void(Arguments...)>& observer);
		void Notify(Arguments... arguments);

	private:
		std::vector<std::function<void(Arguments...)>> m_Observers;

	};

	template<typename ...Arguments>
	void Subject<Arguments...>::AddObserver(const std::function<void(Arguments...)>& observer)
	{
		m_Observers.push_back(observer);
	}

	template<typename ...Arguments>
	void Subject<Arguments...>::RemoveObserver(const std::function<void(Arguments...)>& observer)
	{
		auto iterator	
		{
			std::remove_if
			(
				m_Observers.begin(), m_Observers.end(), [&observer](const std::function<void(Arguments...)>& function) -> bool
				{
					return function.target_type() == observer.target_type() and
					function.target<void(*)(Arguments...)>() == observer.target<void(*)(Arguments...)>();
				}
			)	
		};

		m_Observers.erase(iterator, m_Observers.end());		
	}

	template<typename ...Arguments>
	void Subject<Arguments...>::Notify(Arguments... arguments)
	{
		for (std::function<void(Arguments...)>& observer : m_Observers)
		{
			observer(arguments...);
		}
	}
}