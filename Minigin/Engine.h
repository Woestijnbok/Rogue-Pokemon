#pragma once

#include <string>
#include <functional>
#include <chrono>
#include <vec2.hpp>

struct SDL_Window;

namespace Minigin	
{
	class Engine
	{
	public:
		explicit Engine() = delete;
		~Engine() = delete;

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) noexcept = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) noexcept = delete;

		static void Initialize(const std::string& nameWindow);
		static void Run(const std::function<void()>& load);
		static void Destroy();
		static SDL_Window* GetWindow();
		static glm::ivec2 GetWindowSize();	

	private:
		static glm::ivec2 m_WindowSize;
		static SDL_Window* m_Window;
		static const int m_TargetFrameRate;
		static const std::chrono::milliseconds m_TargetFrameDuration;
		static bool m_Initialized;

	};
}