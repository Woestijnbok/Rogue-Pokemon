#include "TimeManager.h"

using namespace Minigin;

TimeManager::TimeManager() :
	Singleton{},
	m_DeltaTime{ 0 },
	m_FixedDeltaTime{ 20 }
{

}

void TimeManager::SetDeltaTime(std::chrono::milliseconds delta)
{
	m_DeltaTime = delta;
}

std::chrono::duration<float> TimeManager::GetDeltaTime() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(m_DeltaTime);
}

std::chrono::duration<float> TimeManager::GetFixedDeltaTime() const
{
	return std::chrono::duration_cast<std::chrono::duration<float>>(m_FixedDeltaTime);
}