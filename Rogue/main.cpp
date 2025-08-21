#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <functional>
#include <vec2.hpp>

#include "Engine.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"

void Load();

using namespace Minigin;

void Load()
{
	auto scene{ SceneManager::Instance()->CreateScene("Main Menu") };
	scene;
}

int main(int, char* [])
{
	Engine::Initialize("Rogue Pokemon", glm::ivec2{ 920, 520 });
	Engine::Run(&Load);
	Engine::Destroy();

	return 0;
}