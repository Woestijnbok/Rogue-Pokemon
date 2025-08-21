#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <functional>

#include "Engine.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "FPSCounterComponent.h"
#include "SpriteComponent.h"

void Load();

using namespace Minigin;

void Load()
{
	auto scene{ SceneManager::Instance()->CreateScene("Main Menu") };
	scene;
}

int main(int, char* [])
{
	Engine::Initialize("Rogue Pokemon");
	Engine::Run(&Load);
	Engine::Destroy();

	return 0;
}