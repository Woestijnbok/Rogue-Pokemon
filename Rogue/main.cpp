// Libraries
#include <Windows.h>
#include <iostream>
#include <functional>
#include <vec2.hpp>
#ifdef _DEBUG
#include <vld.h>
#endif

// Core
#include "Engine.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"

// Components
#include "TileManagerComponent.h"
#include "BattleManagerComponent.h"
#include "TrainerComponent.h"
#include "MovementComponent.h"
#include "SpriteComponent.h"

// Commands
#include "MoveCommand.h"
#ifdef _DEBUG
#include "DebugCommands.h"
#endif

// Other
#include "Enums.h"
#include "Helpers.h"

#define WIN32_LEAN_AND_MEAN

using namespace Minigin;

void FocusBattle(Scene* world, Scene* battle)
{
	world->SetStatus(ControllableObject::Status::Disabled);
	battle->SetStatus(ControllableObject::Status::Enabled);
}

void FocusWorld(Scene* world, Scene* battle)
{
	world->SetStatus(ControllableObject::Status::Enabled);
	battle->SetStatus(ControllableObject::Status::Disabled);
}

void Load()
{
	Scene* worldScene{ SceneManager::Instance()->CreateScene("World") };
	Scene* battleScene{ SceneManager::Instance()->CreateScene("Battle", false) };

	//

	// Manager game object & components
	GameObject* tileManagerObject{ worldScene->CreateGameObject("Manager", true) };
	TileManagerComponent* tileManagerComponent{ tileManagerObject->CreateComponent<TileManagerComponent>() };

	GameObject* battleManagerObject{ battleScene->CreateGameObject("Manager", true) };
	BattleManagerComponent* battleManagerComponent{ battleManagerObject->CreateComponent<BattleManagerComponent>() };

	tileManagerComponent->OnPokemonEncounter().AddObserver(std::bind(&BattleManagerComponent::MakeBattle, battleManagerComponent, std::placeholders::_1));
	battleManagerComponent->OnBattleStarted().AddObserver(std::bind(&FocusBattle, worldScene, battleScene));
	battleManagerComponent->OnBattleFinished().AddObserver(std::bind(&FocusWorld, worldScene, battleScene));

	// Trainer game object & components
	GameObject* trainerObject{ worldScene->CreateGameObject("Trainer", true) };
	TrainerComponent* trainerComponent{ trainerObject->CreateComponent<TrainerComponent>() }; trainerComponent;
	MovementComponent* movementComponent{ trainerObject->CreateComponent<MovementComponent>(tileManagerComponent) };
	SpriteComponent* spriteComponent{ MakeTrainerSprites(trainerObject, tileManagerComponent, movementComponent) };
	ConnectSpritesToMovement(spriteComponent, movementComponent);
	movementComponent->OnMoveCompleted().AddObserver(std::bind(&TileManagerComponent::CheckForBattle, tileManagerComponent, trainerComponent));

	// Trainer input
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::W, InputAction::Trigger::Pressed, std::make_shared<MoveCommand>(movementComponent, Direction::Up));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::D, InputAction::Trigger::Pressed, std::make_shared<MoveCommand>(movementComponent, Direction::Right));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::S, InputAction::Trigger::Pressed, std::make_shared<MoveCommand>(movementComponent, Direction::Down));
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::A, InputAction::Trigger::Pressed, std::make_shared<MoveCommand>(movementComponent, Direction::Left));

#ifdef _DEBUG
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::B, InputAction::Trigger::Pressed, std::make_shared<SkipBattleCommand>(battleManagerComponent));
#endif
}

int main(int, char* [])
{
	Engine::Initialize("Rogue Pokemon", glm::ivec2{ 960, 480 });
	Engine::Run(&Load);
	Engine::Destroy();

	return 0;
}