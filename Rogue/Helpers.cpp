#include "Helpers.h"

// Libraries
#include <fstream>
#include <format>

// Core
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameObject.h"

// Components
#include "PokemonComponent.h"
#include "TileManagerComponent.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"
#include "TrainerComponent.h"

// Other
#include "Pokedex.hpp"

using namespace Minigin;

PokemonComponent* ReadPokemon(uint8_t pokedexIndex, TrainerComponent* trainer)
{
	const std::filesystem::path path{ ResourceManager::Instance()->GetFileRootPath() / "Pokedex.bin" };
	assert(std::filesystem::exists(path));

	std::ifstream file{ path, std::ios::binary };
	assert(file.is_open());

	file.seekg(sizeof(PODPokemon) * (pokedexIndex - 1));

	PODPokemon pokemon{};
	file.read(reinterpret_cast<char*>(&pokemon), sizeof(PODPokemon));

	Scene* battleScene{ SceneManager::Instance()->GetScene("Battle") };
	GameObject* pokemonObject{ nullptr };
	PokemonComponent* pokemonComponent{ nullptr };
	if (trainer == nullptr)
	{
		pokemonObject = battleScene->CreateGameObject(std::format("Wild {}", pokemon.Name));
		pokemonComponent = pokemonObject->CreateComponent<PokemonComponent>(pokemon);
	}
	else
	{
		pokemonObject = battleScene->CreateGameObject(std::format("Tamed {}", pokemon.Name));
		pokemonComponent = pokemonObject->CreateComponent<PokemonComponent>(pokemon, trainer);
	}
	

	return pokemonComponent;
}

SpriteComponent* MakeTrainerSprites(GameObject* object, TileManagerComponent* tileManager, MovementComponent* movementComponent)
{
	SpriteComponent* spriteComponent{ object->CreateComponent<SpriteComponent>() };

	// TODO: take a look at how hardcoded values can be avoided here
	constexpr int frames{ 3 };
	constexpr glm::vec2 feetOffsetUpDown{ 0, 5 };
	constexpr glm::vec2 feetOffsetRight{ -2, 5 };
	const std::chrono::milliseconds frameTime{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(tileManager->GetTileSize() / (movementComponent->GetSpeed() * frames))) };
	const float renderScale{ tileManager->GetRenderScale() };

	const SpriteInformation downSpriteInformation
	{
		ResourceManager::Instance()->LoadSprite("Character/Down.png", frames, 3, 1),
		frameTime,
		Transform{ glm::ivec2{ feetOffsetUpDown * renderScale }, 0, glm::vec2{ renderScale } }
	};
	spriteComponent->AddSprite("Down", downSpriteInformation);

	const SpriteInformation rightSpriteInformation
	{
		ResourceManager::Instance()->LoadSprite("Character/Right.png", frames, 3, 1),
		frameTime,
		Transform{ glm::ivec2{ feetOffsetRight * renderScale }, 0, glm::vec2{ renderScale } }
	};
	spriteComponent->AddSprite("Right", rightSpriteInformation);

	const SpriteInformation upSpriteInformation
	{
		ResourceManager::Instance()->LoadSprite("Character/Up.png", frames, 3, 1),
		frameTime,
		Transform{ glm::ivec2{ feetOffsetUpDown * renderScale }, 0, glm::vec2{ renderScale } }
	};
	spriteComponent->AddSprite("Up", upSpriteInformation);

	spriteComponent->SetSprite("Down"); 

	spriteComponent->SetPaused(true);
	spriteComponent->SetLoop(false);

	return spriteComponent;
}

void ConnectSpritesToMovement(SpriteComponent* spriteComponent, MovementComponent* movementComponent)
{
	movementComponent->OnMoveCompleted().AddObserver([spriteComponent]() -> void { spriteComponent->SetPaused(true); });

	movementComponent->OnMoveStarted().AddObserver
	(
		[spriteComponent](Direction direction) -> void
		{ 
			switch (direction)
			{
			case Direction::Up:
				spriteComponent->SetSprite("Up");
				break;
			case Direction::Right:
				spriteComponent->SetSprite("Right");
				// TODO: make sprite flipper helper function
				{
					const Transform spriteRenderOffset{ spriteComponent->GetCurrentSprite()->RenderOffset.value() };
					spriteComponent->GetCurrentSprite()->RenderOffset.value().SetScale(glm::vec2{ std::abs(spriteRenderOffset.GetScale().x), spriteRenderOffset.GetScale().y });
					spriteComponent->GetCurrentSprite()->RenderOffset.value().SetPosition(glm::ivec2{ -std::abs(spriteRenderOffset.GetPosition().x), spriteRenderOffset.GetPosition().y });
				}
				break;
			case Direction::Down:
				spriteComponent->SetSprite("Down");
				break;
			case Direction::Left:
				spriteComponent->SetSprite("Right");

				{
					const Transform spriteRenderOffset{ spriteComponent->GetCurrentSprite()->RenderOffset.value() };
					spriteComponent->GetCurrentSprite()->RenderOffset.value().SetScale(glm::vec2{ -std::abs(spriteRenderOffset.GetScale().x), spriteRenderOffset.GetScale().y });
					spriteComponent->GetCurrentSprite()->RenderOffset.value().SetPosition(glm::ivec2{ std::abs(spriteRenderOffset.GetPosition().x), spriteRenderOffset.GetPosition().y });
				}
				break;
			default:
				assert(false);
			}
		}
	);
}

std::filesystem::path GetPokemonTexturePath(const std::string& name, bool isWild)
{
	const std::string textureName{ std::format("Pokemon/{}{}", name, (isWild) ? ".png" : " Back.png" ) };

	return ResourceManager::Instance()->GetTextureRootPath() / textureName;
}