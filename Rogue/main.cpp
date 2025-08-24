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
#include "MovementComponent.h"
#include "MoveCommand.h"
#include "SpriteComponent.h"

void Load();

using namespace Minigin;

void Load()
{
	Scene* scene{ SceneManager::Instance()->CreateScene("World") };

	// Manager game object & components
	GameObject* managerObject{ scene->CreateGameObject("Manager", true) };
	TileManagerComponent* tileManagerComponent{ managerObject->CreateComponent<TileManagerComponent>() };

	// Trainer game object & components
	GameObject* trainerObject{ scene->CreateGameObject("Trainer", true) };
	//TrainerComponent* trainerComponent{ trainerObject->CreateComponent<TrainerComponent>() };
	SpriteComponent* spriteComponent{ trainerObject->CreateComponent<SpriteComponent>() }; spriteComponent;
	MovementComponent* movementComponent{ trainerObject->CreateComponent<MovementComponent>(tileManagerComponent) };

	// Trainer input
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::W, InputAction::Trigger::Pressed, std::make_shared<MoveCommand>(movementComponent, MovementComponent::Direction::Up));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::D, InputAction::Trigger::Pressed, std::make_shared<MoveCommand>(movementComponent, MovementComponent::Direction::Right));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::S, InputAction::Trigger::Pressed, std::make_shared<MoveCommand>(movementComponent, MovementComponent::Direction::Down));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::A, InputAction::Trigger::Pressed, std::make_shared<MoveCommand>(movementComponent, MovementComponent::Direction::Left));
}

int main(int, char* [])
{
	Engine::Initialize("Rogue Pokemon", glm::ivec2{ 960, 480 });
	Engine::Run(&Load);
	Engine::Destroy();

	return 0;
}