#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdexcept>
#include <SDL.h>
#include <thread>
#include <iostream>

#include "Engine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "TimeManager.h"

using namespace Minigin;

glm::ivec2 Engine::m_WindowSize{ 510, 600 };
SDL_Window* Engine::m_Window{ nullptr };
const int Engine::m_TargetFrameRate{ 60 };
const std::chrono::milliseconds Engine::m_TargetFrameDuration{ 1000 / m_TargetFrameRate };
bool Engine::m_Initialized{ false };

void Engine::Initialize(const std::string& nameWindow)
{
	if (m_Initialized) return;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow
	(
		nameWindow.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowSize.x, m_WindowSize.y,
		SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_MOUSE_GRABBED | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_INPUT_GRABBED
		| SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_CAPTURE
	);

	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	// Initialize engine features
	Renderer::Instance();
	AudioManager::Instance();	
	ResourceManager::Instance();
	SceneManager::Instance();
	InputManager::Instance();
	TimeManager::Instance();

	m_Initialized = true;

	std::cout << "Use the escape key to exit the game." << std::endl;
}

void Engine::Run(const std::function<void()>& load)
{
	load();

	// Cashe engine features
	auto inputManager{ InputManager::Instance() };
	auto sceneManager{ SceneManager::Instance() };
	auto renderer{ Renderer::Instance() };
	auto timeManager{ TimeManager::Instance() };
	auto audioManager{ AudioManager::Instance() };

	std::thread audioThread{ &AudioManager::Update, audioManager };			
	audioThread.detach();			

	bool exit{ false };
	std::chrono::steady_clock::time_point lastTime{ std::chrono::high_resolution_clock::now() };
	std::chrono::milliseconds lag{};
	while (!exit)
	{
		const std::chrono::steady_clock::time_point currentTime{ std::chrono::high_resolution_clock::now() };
		const std::chrono::milliseconds deltaTime{ std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime) };
		lastTime = currentTime;
		lag += deltaTime;
		timeManager->SetDeltaTime(deltaTime);

		exit = inputManager->ProcessInput();	

		while (lag >= timeManager->GetFixedDeltaTime())	
		{
			sceneManager->FixedUpdate();	
			lag -= std::chrono::duration_cast<std::chrono::milliseconds>(timeManager->GetFixedDeltaTime());
		}
		sceneManager->Update();	
		sceneManager->LateUpdate();	

		renderer->Render();	

		std::this_thread::sleep_for(currentTime + m_TargetFrameDuration - std::chrono::high_resolution_clock::now());
	}

	audioManager->StopRunning();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));	
}

void Engine::Destroy()
{
	Renderer::Destroy();
	SDL_DestroyWindow(m_Window);
	AudioManager::Destroy();
	ResourceManager::Destroy();
	SceneManager::Destroy();
	InputManager::Destroy();
	TimeManager::Destroy();
	SDL_Quit();
}

SDL_Window* Minigin::Engine::GetWindow()
{
	return m_Window;
}

glm::ivec2 Minigin::Engine::GetWindowSize()
{
	return m_WindowSize;
}