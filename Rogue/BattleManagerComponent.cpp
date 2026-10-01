#include "BattleManagerComponent.h"

// Libraries
#include <cassert>
#include <algorithm>

// Core
#include "Engine.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h"
#include "Text.h"
#include "Scene.h"

// Components
#include "PokemonComponent.h"
#include "TrainerComponent.h"
#include "MovementComponent.h"

// Other
#include "Helpers.h"
#include "Pokedex.hpp"

using namespace Minigin;

BattleManagerComponent::BattleManagerComponent(GameObject* owner) :
	Component{ owner },
	m_CurrentBattle{ nullptr, nullptr },
	m_CurrentBattleState{ BattleState::None },
	m_OnBattleStarted{},
	m_OnBattleFinished{},
	m_OnBattleWon{},
	m_OnBattleLost{},
	m_BattleBackground{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Battle Background.png") },
	m_TrainerCloud{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Trainer Cloud.png") },
	m_EnemyCloud{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Enemy Cloud.png") },
	m_InfoBox{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Info Box.png") },
	m_MoveBox{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Move Box.png") },
	m_SelectArrow{ Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / "Select Arrow.png") },
	m_WildText{ new Text{ "Wild", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_UsedText{ new Text{ "used", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_RandomDevice{},
	m_RandomEngine{ m_RandomDevice() },
	m_ChanceDistribution{ 0.0f, 100.0f },
	m_CommonDistribution{ 1, POKEDEX_LEGENDARY_START - 1 },
	m_LegendaryDistribution{ POKEDEX_LEGENDARY_START, POKEDEX_COUNT },
	m_LegendaryChance{ 5 },
	m_CurrentMove{ 0 }
{
	assert(m_BattleBackground.get());
}

void BattleManagerComponent::Render() const
{
	RenderBackground();
	RenderPokemons();
	RenderTrainerCloud();
	RenderEnemyCloud();
	RenderInfoBox();

	switch (m_CurrentBattleState)
	{
	case BattleState::SelectMove:
		RenderMoveSelect();
		break;
	case BattleState::PerformPlayerMove:
		RenderUsedMove(true);
		break;
	case BattleState::PerformEnemyMove:
		RenderUsedMove(false);
		break;
	case BattleState::EndBattle:
		RenderUsedMove(m_CurrentBattle.second->IsDead());
		break;
	case BattleState::None:
		break;
	default:
		break;
	}
}

void BattleManagerComponent::MakeBattle(TrainerComponent* trainer)
{
	uint8_t enemyPokemonIndex{ 0 };
	if (m_ChanceDistribution(m_RandomEngine) <= m_LegendaryChance)
	{
		enemyPokemonIndex = static_cast<uint8_t>(m_LegendaryDistribution(m_RandomEngine));
	}
	else
	{
		enemyPokemonIndex = static_cast<uint8_t>(m_CommonDistribution(m_RandomEngine));
	}

	PODPokemon pokemon{};
	ReadPokemon(pokemon, enemyPokemonIndex);

	GameObject* wildPokemonObject{ GetOwner()->GetScene()->CreateGameObject(std::format("Wild {}", pokemon.Name)) };
	PokemonComponent* wildPokemonComponent{ wildPokemonObject->CreateComponent<PokemonComponent>(pokemon) };

	StartBattle(trainer, wildPokemonComponent);
}

void BattleManagerComponent::EndBattle()
{
	m_CurrentBattle.second->GetOwner()->SetStatus(ControllableObject::Status::Destroyed);
	m_CurrentBattleState = BattleState::None;

	// Notify occured events.
	m_OnBattleFinished.Notify();
	if (m_CurrentBattle.second->IsDead())
	{
		m_OnBattleWon.Notify();
	}
	else
	{
		m_OnBattleLost.Notify();
	}

	m_CurrentBattle.first = nullptr;
	m_CurrentBattle.second = nullptr;
}

bool BattleManagerComponent::InBattle() const
{
	return m_CurrentBattle.second != nullptr;
}

void BattleManagerComponent::ChangeSelectedMove(Direction direction)
{
	switch (direction)
	{
	case Direction::Up:
		if (m_CurrentMove == 2 or m_CurrentMove == 3)
		{
			m_CurrentMove -= 2;
		}
		break;
	case Direction::Right:
		if (m_CurrentMove == 0 or m_CurrentMove == 2)
		{
			m_CurrentMove += 1;
		}
		break;
	case Direction::Down:
		if (m_CurrentMove == 0 or m_CurrentMove == 1)
		{
			m_CurrentMove += 2;
		}
		break;
	case Direction::Left:
		if (m_CurrentMove == 1 or m_CurrentMove == 3)
		{
			m_CurrentMove -= 1;
		}
		break;
	}

	assert(m_CurrentMove >= 0 and m_CurrentMove < 4);
}

void BattleManagerComponent::Confirm()
{
	if (m_CurrentBattleState != BattleState::None)
	{
		switch (m_CurrentBattleState)
		{
		case BattleState::SelectMove:
			m_CurrentBattleState = BattleState::PerformPlayerMove;
			UseSelectedMove();
			CheckBattleEnd();
			break;
		case BattleState::PerformPlayerMove:
			m_CurrentBattleState = BattleState::PerformEnemyMove;
			SelectEnemyMove();
			UseSelectedMove();
			CheckBattleEnd();
			break;
		case BattleState::PerformEnemyMove:
			m_CurrentBattleState = BattleState::SelectMove;
			m_CurrentMove = 0;
			break;
		case BattleState::EndBattle:
			EndBattle();
			break;
		case BattleState::None:
			break;
		default:
			break;
		}
	}
}

Minigin::Subject<>& BattleManagerComponent::OnBattleStarted()
{
	return m_OnBattleStarted;
}

Minigin::Subject<>& BattleManagerComponent::OnBattleFinished()
{
	return m_OnBattleFinished;
}

Minigin::Subject<>& BattleManagerComponent::OnBattleWon()
{
	return m_OnBattleWon;
}

Minigin::Subject<>& BattleManagerComponent::OnBattleLost()
{
	return m_OnBattleLost;
}

Texture const* BattleManagerComponent::GetInfoBoxTexture() const
{
	return m_InfoBox.get();
}

void BattleManagerComponent::StartBattle(TrainerComponent* trainer, PokemonComponent* enemy)
{
	m_CurrentBattle.first = trainer;
	m_CurrentBattle.second = enemy;
	m_CurrentBattleState = BattleState::SelectMove;

	m_OnBattleStarted.Notify();
}

void BattleManagerComponent::RenderBackground() const
{
	const glm::ivec2 windowSize{ Engine::GetWindowSize() };
	const glm::ivec2 textureSize{ m_BattleBackground->GetSize() };

	const Transform transform{ windowSize / 2 , 0, glm::vec2{ windowSize.x / float(textureSize.x), windowSize.y / float(textureSize.y) } };
	Renderer::Instance()->RenderTexture(*m_BattleBackground, transform);
}

void BattleManagerComponent::RenderPokemons() const
{
	const Transform trainerTransform{ glm::ivec2{ 200, 95 } , 0, glm::vec2{ 3.0f } };
	Renderer::Instance()->RenderTexture(*m_CurrentBattle.first->GetActivePokemon()->GetTexture(), trainerTransform);

	const Transform enemyTransform{ glm::ivec2{ 700, 290 }, 0, glm::vec2{ 3.0f } };
	Renderer::Instance()->RenderTexture(*m_CurrentBattle.second->GetTexture(), enemyTransform);
}

void BattleManagerComponent::RenderTrainerCloud() const
{
	RenderTrainerHealth();

	PokemonComponent const * trainerPokemon{ m_CurrentBattle.first->GetActivePokemon() };

	const Transform cloudTransform{ glm::ivec2{ 500, 150 }, 0, glm::vec2{ 3.0f } };
	Renderer::Instance()->RenderTexture(*m_TrainerCloud, cloudTransform);

	const int nameWidth{ trainerPokemon->GetNameText()->GetTexture()->GetSize().x };
	const Transform nameTransform{ glm::ivec2{ 390 + (nameWidth / 2), 170 }, 0, glm::vec2{ 1.0f } };
	Renderer::Instance()->RenderText(*trainerPokemon->GetNameText(), nameTransform);

	const int levelWidth{ trainerPokemon->GetLevelText()->GetTexture()->GetSize().x };
	const Transform levelTransform{ glm::ivec2{ 630 - (levelWidth / 2), 170 }, 0, glm::vec2{ 1.0f } };
	Renderer::Instance()->RenderText(*trainerPokemon->GetLevelText(), levelTransform);
}

void BattleManagerComponent::RenderTrainerHealth() const
{
	PokemonComponent const* trainerPokemon{ m_CurrentBattle.first->GetActivePokemon() };

	constexpr glm::ivec2 bottomLeft{ 460, 130 };
	constexpr glm::ivec2 topRight{ 620, 155 };
	constexpr Color backgroundColor{ 76, 74, 76 };

	const float healthPercentage{ trainerPokemon->GetHealthPercentage() };
	const int healthWidth{ static_cast<int>((topRight.x - bottomLeft.x) * healthPercentage) };
	const glm::ivec2 healthTopRight{ bottomLeft.x + healthWidth, topRight.y };
	const glm::ivec2 backgroundBottomLeft{ healthTopRight.x, bottomLeft.y };

	Color healthColor{ Color::Green };
	if (healthPercentage <= 0.25f)
	{
		healthColor = Color::Red;
	}
	else if (healthPercentage <= 0.5f)
	{
		healthColor = Color{ 255, 165, 0 };
	}

	Renderer::Instance()->RenderBox(backgroundBottomLeft , topRight, backgroundColor, true);
	Renderer::Instance()->RenderBox(bottomLeft, healthTopRight, healthColor, true);
}

void BattleManagerComponent::RenderEnemyCloud() const
{
	RenderEnemyHealth();

	const Transform cloudTransform{ glm::ivec2{ 350, 350 }, 0, glm::vec2{ 3.0f, 3.0f } };
	Renderer::Instance()->RenderTexture(*m_EnemyCloud, cloudTransform);

	const int nameWidth{ m_CurrentBattle.second->GetNameText()->GetTexture()->GetSize().x };
	const Transform nameTransform{ glm::ivec2{ 220 + (nameWidth / 2), 370}, 0, glm::vec2{1.0f}};
	Renderer::Instance()->RenderText(*m_CurrentBattle.second->GetNameText(), nameTransform);

	const int levelWidth{ m_CurrentBattle.second->GetLevelText()->GetTexture()->GetSize().x };
	const Transform levelTransform{ glm::ivec2{ 460 - (levelWidth / 2), 370 }, 0, glm::vec2{ 1.0f } };
	Renderer::Instance()->RenderText(*m_CurrentBattle.second->GetLevelText(), levelTransform);
}

void BattleManagerComponent::RenderEnemyHealth() const
{
	constexpr glm::ivec2 bottomLeft{ 310, 330 };
	constexpr glm::ivec2 topRight{ 470, 355 };
	constexpr Color backgroundColor{ 76, 74, 76 };

	const float healthPercentage{ m_CurrentBattle.second->GetHealthPercentage() };
	const int healthWidth{ static_cast<int>((topRight.x - bottomLeft.x) * healthPercentage) };
	const glm::ivec2 healthTopRight{ bottomLeft.x + healthWidth, topRight.y };
	const glm::ivec2 backgroundBottomLeft{ healthTopRight.x, bottomLeft.y };

	Color healthColor{ Color::Green };
	if (healthPercentage <= 0.25f)
	{
		healthColor = Color::Red;
	}
	else if (healthPercentage <= 0.5f)
	{
		healthColor = Color{ 255, 165, 0 };
	}

	Renderer::Instance()->RenderBox(backgroundBottomLeft, topRight, backgroundColor, true);
	Renderer::Instance()->RenderBox(bottomLeft, healthTopRight, healthColor, true);
}

void BattleManagerComponent::RenderInfoBox() const
{
	const Transform boxTransform{ glm::ivec2{ 745, 41 }, 0, glm::vec2{ 1.8f } };
	Renderer::Instance()->RenderTexture(*m_MoveBox, boxTransform);
}

void BattleManagerComponent::RenderMoveSelect() const
{
	const std::array<Move, 4>& trainerPokemonMoves{ m_CurrentBattle.first->GetActivePokemon()->GetMoves()};

	const int firstMoveWidth{ trainerPokemonMoves.at(0).Name->GetTexture()->GetSize().x };
	const glm::ivec2 firstMovePosition{ 580, 60 };
	const Transform firstMoveTransform{ glm::ivec2{ firstMovePosition.x + (firstMoveWidth / 2), firstMovePosition.y }, 0, glm::vec2{1.0f}};
	Renderer::Instance()->RenderText(*trainerPokemonMoves.at(0).Name.get(), firstMoveTransform);

	const int secondMoveWidth{ trainerPokemonMoves.at(1).Name->GetTexture()->GetSize().x };
	const glm::ivec2 secondMovePosition{ 780, 60 };
	const Transform secondMoveTransform{ glm::ivec2{ secondMovePosition.x + (secondMoveWidth / 2), secondMovePosition.y }, 0, glm::vec2{1.0f} };
	Renderer::Instance()->RenderText(*trainerPokemonMoves.at(1).Name.get(), secondMoveTransform);

	const int thirdMoveWidth{ trainerPokemonMoves.at(2).Name->GetTexture()->GetSize().x };
	const glm::ivec2 thirdMovePosition{ 580, 25 };
	const Transform thirdMoveTransform{ glm::ivec2{ thirdMovePosition.x + (thirdMoveWidth / 2), thirdMovePosition.y }, 0, glm::vec2{1.0f} };
	Renderer::Instance()->RenderText(*trainerPokemonMoves.at(2).Name.get(), thirdMoveTransform);

	const int fourthMoveWidth{ trainerPokemonMoves.at(3).Name->GetTexture()->GetSize().x };
	const glm::ivec2 fourthMovePosition{ 780, 25 };
	const Transform fourthMoveTransform{ glm::ivec2{ fourthMovePosition.x + (fourthMoveWidth / 2), fourthMovePosition.y }, 0, glm::vec2{1.0f} };
	Renderer::Instance()->RenderText(*trainerPokemonMoves.at(3).Name.get(), fourthMoveTransform);

	glm::ivec2 arrowPosition{};
	switch (m_CurrentMove)
	{
	case 0:
		arrowPosition = glm::ivec2{ 565.0f, 60.0f };
		break;
	case 1:
		arrowPosition = glm::ivec2{ 765.0f, 60.0f };
		break;
	case 2:
		arrowPosition = glm::ivec2{ 565.0f, 25.0f };
		break;
	case 3:
		arrowPosition = glm::ivec2{ 765.0f, 25.0f };
		break;
	default:
		throw std::runtime_error("BattleManagerComponent::RenderMoveSelect: Invalid move index");
	}

	Renderer::Instance()->RenderTexture(*m_SelectArrow.get(), Transform{ arrowPosition, 0, glm::vec2{ 1.0f } });
}

void BattleManagerComponent::RenderUsedMove(bool isPlayerMove) const
{
	const glm::ivec2 topPosition{ 550.0f, 45.0f };
	const glm::ivec2 bottomPosition{ 550.0f, 15.0f };
	const uint8_t whiteSpace{ 10 };

	if (isPlayerMove)
	{
		PokemonComponent const* trainerPokemon{ m_CurrentBattle.first->GetActivePokemon() };

		const glm::ivec2 namePosition{ topPosition + (trainerPokemon->GetNameText()->GetTexture()->GetSize() / 2) };
		Renderer::Instance()->RenderText(*trainerPokemon->GetNameText(), Transform{ namePosition, 0, glm::vec2{ 1.0f } });

		const glm::ivec2 usedPosition{ topPosition.x + trainerPokemon->GetNameText()->GetTexture()->GetSize().x + whiteSpace + (m_UsedText->GetTexture()->GetSize().x / 2), topPosition.y + (m_UsedText->GetTexture()->GetSize().y / 2) };
		Renderer::Instance()->RenderText(*m_UsedText, Transform{ usedPosition, 0, glm::vec2{ 1.0f } });

		const glm::ivec2 movePosition{ bottomPosition + (trainerPokemon->GetMoves().at(m_CurrentMove).Name->GetTexture()->GetSize() / 2) };
		Renderer::Instance()->RenderText(*trainerPokemon->GetMoves().at(m_CurrentMove).Name, Transform{movePosition, 0, glm::vec2{1.0f}});
	}
	else
	{
		const glm::ivec2 wildPosition{ topPosition + (m_WildText->GetTexture()->GetSize() / 2) };
		Renderer::Instance()->RenderText(*m_WildText, Transform{ wildPosition, 0, glm::vec2{ 1.0f } });

		const glm::ivec2 namePosition{ topPosition.x + (m_WildText->GetTexture()->GetSize().x) + whiteSpace + (m_CurrentBattle.second->GetNameText()->GetTexture()->GetSize().x / 2), topPosition.y + (m_CurrentBattle.second->GetNameText()->GetTexture()->GetSize().y / 2) };
		Renderer::Instance()->RenderText(*m_CurrentBattle.second->GetNameText(), Transform{ namePosition, 0, glm::vec2{ 1.0f } });

		const glm::ivec2 usedPosition{ topPosition.x + (m_WildText->GetTexture()->GetSize().x) + (m_CurrentBattle.second->GetNameText()->GetTexture()->GetSize().x) + (2 * whiteSpace) + (m_UsedText->GetTexture()->GetSize().x / 2), topPosition.y + (m_UsedText->GetTexture()->GetSize().y / 2) };
		Renderer::Instance()->RenderText(*m_UsedText, Transform{ usedPosition, 0, glm::vec2{ 1.0f } });

		const glm::ivec2 movePosition{ bottomPosition + (m_CurrentBattle.second->GetMoves().at(m_CurrentMove).Name->GetTexture()->GetSize() / 2) };
		Renderer::Instance()->RenderText(*m_CurrentBattle.second->GetMoves().at(m_CurrentMove).Name, Transform{ movePosition, 0, glm::vec2{1.0f} });
	}
}

void BattleManagerComponent::UseSelectedMove()
{
	PokemonComponent* trainerPokemon{ m_CurrentBattle.first->GetActivePokemon() };
	PokemonComponent* wildPokemon{ m_CurrentBattle.second };

	if (m_CurrentBattleState == BattleState::PerformPlayerMove)
	{
		uint8_t attackPower{ trainerPokemon->GetMoves()[m_CurrentMove].Power};
		if (wildPokemon->GetStats().CurrentHealth < attackPower)
		{
			wildPokemon->GetStats().CurrentHealth = 0;
		}
		else
		{
			wildPokemon->GetStats().CurrentHealth -= attackPower;
		}
	}
	else
	{
		uint8_t attackPower{ wildPokemon->GetMoves()[m_CurrentMove].Power };
		if (trainerPokemon->GetStats().CurrentHealth < attackPower)
		{
			trainerPokemon->GetStats().CurrentHealth = 0;
		}
		else
		{
			trainerPokemon->GetStats().CurrentHealth -= attackPower;
		}
	}
}

void BattleManagerComponent::SelectEnemyMove()
{
	// For now just select a random move, later we can implement some scoring system to select the best move based on the situation.
	std::uniform_int_distribution<int> distribution{ 0, 3 };
	m_CurrentMove = static_cast<uint8_t>(distribution(m_RandomEngine));
}

void BattleManagerComponent::CheckBattleEnd()
{
	if (m_CurrentBattle.first->GetActivePokemon()->IsDead() or m_CurrentBattle.second->IsDead())
	{
		m_CurrentBattleState = BattleState::EndBattle;
	}
}