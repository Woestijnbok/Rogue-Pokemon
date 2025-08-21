#pragma once

#include <chrono>

#include "Singleton.h"

namespace Minigin
{
	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		friend class Singleton<TimeManager>;

		~TimeManager() = default;

		TimeManager(const TimeManager& other) = delete;
		TimeManager(TimeManager&& other) = delete;
		TimeManager& operator=(const TimeManager& other) = delete;
		TimeManager& operator=(TimeManager&& other) = delete;

		void SetDeltaTime(std::chrono::milliseconds delta);
		std::chrono::duration<float> GetDeltaTime() const;
		std::chrono::duration<float> GetFixedDeltaTime() const;	

	private:
		std::chrono::milliseconds m_DeltaTime;	
		const std::chrono::milliseconds m_FixedDeltaTime;	

		explicit TimeManager();

	};
}