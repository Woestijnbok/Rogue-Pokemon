#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <functional>
#include <vec2.hpp>

#include "Engine.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "TileManagerComponent.h"
#include "TrainerComponent.h"

void Load();

using namespace Minigin;

void Load()
{
	Scene* scene{ SceneManager::Instance()->CreateScene("World") };

	GameObject* manager{ scene->CreateGameObject("Manager", true) };
	manager->CreateComponent<TileManagerComponent>();

	GameObject* trainerObject{ scene->CreateGameObject("Trainer", true) };
	TrainerComponent* trainerComponent{ trainerObject->CreateComponent<TrainerComponent>() };

	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::W, InputAction::Trigger::Down, std::make_shared<MoveCommand>(trainerComponent, MoveCommand::Direction::Up));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::D, InputAction::Trigger::Down, std::make_shared<MoveCommand>(trainerComponent, MoveCommand::Direction::Right));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::S, InputAction::Trigger::Down, std::make_shared<MoveCommand>(trainerComponent, MoveCommand::Direction::Down));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::A, InputAction::Trigger::Down, std::make_shared<MoveCommand>(trainerComponent, MoveCommand::Direction::Left));
}

int main(int, char* [])
{
	Engine::Initialize("Rogue Pokemon", glm::ivec2{ 960, 480 });
	Engine::Run(&Load);
	Engine::Destroy();

	return 0;
}