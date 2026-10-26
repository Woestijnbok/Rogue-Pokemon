#include "BattleManagerComponent.h"

#define POKEMON_MOVE_SCORING_EPSILON 0.1f

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
#include "PokedexComponent.h"
#include "Move.h"

// Other
#include "Helpers.h"
#include "Pokedex.hpp"

using namespace Minigin;

BattleManagerComponent::BattleManagerComponent(GameObject* owner) :
	Component{ owner },
	m_Pokedex{ owner->GetComponent<PokedexComponent>() },
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
	m_ElementIcons{},
	m_WildText{ new Text{ "Wild", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_UsedText{ new Text{ "used", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_MissedMoveText{ new Text{ "But it failed!", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_ACriticalMoveText{ new Text{ "A critical hit!", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_SuperEffectiveMoveText{ new Text{ "It's super effective!", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_NotVeryEffectiveMoveText{ new Text{ "It's not very effective...", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_ImmuneMoveText{ new Text{ "It has no effect", ResourceManager::Instance()->GetOrLoadFont("Emerald.ttf", "Emerald", 30), Color::Black } },
	m_RandomDevice{},
	m_RandomEngine{ m_RandomDevice() },
	m_StartWildPokemonLevelDistribution{ 1, 5 },
	m_CurrentWildPokemonLevelDistribution{ m_StartWildPokemonLevelDistribution },
	m_ChanceDistribution{ 0.0f, 100.0f },
	m_CommonDistribution{ 1, POKEDEX_POKEMON_LEGENDARY_START - 1 },
	m_LegendaryDistribution{ POKEDEX_POKEMON_LEGENDARY_START, POKEDEX_POKEMON_COUNT },
	m_StartLegendaryChance{ 1.0f },
	m_LegendaryChance{ m_StartLegendaryChance },
	m_StartEnemySmartness{ 30.0f },
	m_EnemySmartness{ m_StartEnemySmartness },
	m_CriticalChance{ 4.0f },
	m_CriticalHitMultiplier{ 1.5f },
	m_LastMoveEffectiveness{ 1.0f },
	m_CurrentMove{ 0 },
	m_IsPlayerTurn{ true },
	m_LastMoveHit{ true },
	m_LastMoveCrit{ false }
{
	SetRenderOrder(RenderOrder::UI);
	LoadElementIcons();
}

void BattleManagerComponent::Render() const
{
	RenderBackground();
	RenderPokemons();
	RenderCloud(*m_CurrentBattle.first->GetActivePokemon(), *m_TrainerCloud, glm::ivec2{ 500, 150 }, -110, 130);
	RenderCloud(*m_CurrentBattle.second, *m_EnemyCloud, glm::ivec2{ 350, 350 }, -130, 110);
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
	case BattleState::MissedMove:
		RenderMessage(*m_MissedMoveText);
		break;
	case BattleState::CritMove:
		RenderMessage(*m_ACriticalMoveText);
		break;
	case BattleState::Effectiveness:
		if (m_LastMoveEffectiveness == 0.0f) RenderMessage(*m_ImmuneMoveText);
		else if (m_LastMoveEffectiveness > 1.0f) RenderMessage(*m_SuperEffectiveMoveText);
		else RenderMessage(*m_NotVeryEffectiveMoveText);
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

	PODPokemon pokemon{ m_Pokedex->GetPokemon(enemyPokemonIndex) };
	GameObject* wildPokemonObject{ GetOwner()->GetScene()->CreateGameObject(std::format("Wild {}", pokemon.Name)) };
	PokemonComponent* wildPokemonComponent{ wildPokemonObject->CreateComponent<PokemonComponent>(pokemon) };
	SetWildPokemonLevel(wildPokemonComponent);

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
		m_CurrentBattle.first->GetActivePokemon()->GainExperience(m_CurrentBattle.second->GetLevel());
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
	if(m_CurrentBattleState == BattleState::SelectMove)
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
			PerformTurn(true);
			break;
		case BattleState::PerformPlayerMove:
		case BattleState::PerformEnemyMove:
			if (!m_LastMoveHit)
			{
				m_CurrentBattleState = BattleState::MissedMove;
			}
			else if (m_LastMoveCrit)
			{
				m_CurrentBattleState = BattleState::CritMove;
			}
			else if (m_LastMoveEffectiveness != 1.0f)
			{
				m_CurrentBattleState = BattleState::Effectiveness;
			}
			else
			{
				AdvanceTurn();
			}
			break;
		case BattleState::MissedMove:
		case BattleState::Effectiveness:
			AdvanceTurn();
			break;
		case BattleState::CritMove:
			if (m_LastMoveEffectiveness != 1.0f)
			{
				m_CurrentBattleState = BattleState::Effectiveness;
			}
			else
			{
				AdvanceTurn();
			}
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

void BattleManagerComponent::Reset()
{
	m_CurrentWildPokemonLevelDistribution = m_StartWildPokemonLevelDistribution;
	m_LegendaryChance = m_StartLegendaryChance;
	m_EnemySmartness = m_StartEnemySmartness;
}

std::uniform_int_distribution<int>::param_type BattleManagerComponent::GetWildPokemonLevelRange()
{
	return m_CurrentWildPokemonLevelDistribution.param();
}

void BattleManagerComponent::SetWildPokemonLevelRange(const std::uniform_int_distribution<int>::param_type& range)
{
	if ((range.a() > range.b()) or (range.a() < 0) or (range.b() > 100))
	{
		throw std::exception{ std::format("Invalid wild pokemon level range: minLevel = {}, maxLevel = {}", range.a(), range.b()).c_str()};
	}

	m_CurrentWildPokemonLevelDistribution.param(range);
}

void BattleManagerComponent::IncreaseDifficulty()
{
	constexpr int newLevelIncrease{ 5 };
	std::uniform_int_distribution<int>::param_type newWildPokemonLevelRange{ m_CurrentWildPokemonLevelDistribution.a() + newLevelIncrease, m_CurrentWildPokemonLevelDistribution.b() + newLevelIncrease };
	SetWildPokemonLevelRange(newWildPokemonLevelRange);

	m_LegendaryChance = std::clamp(m_LegendaryChance + 1.0f, 0.0f, 100.0f);
	m_EnemySmartness = std::clamp(m_EnemySmartness - 1.0f, 0.0f, 100.0f);
}

void BattleManagerComponent::StartBattle(TrainerComponent* trainer, PokemonComponent* enemy)
{
	m_CurrentBattle.first = trainer;
	m_CurrentBattle.second = enemy;

	if (trainer->GetActivePokemon()->GetStats().Speed >= enemy->GetStats().Speed)
	{
		m_CurrentBattleState = BattleState::SelectMove;
		m_IsPlayerTurn = true;
	}
	else
	{
		PerformTurn(false);
	}

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

void BattleManagerComponent::RenderCloud(const PokemonComponent& pokemon, const Minigin::Texture& cloud, const glm::ivec2& cloudCenter, int nameLeftOffset, int levelRightOffset) const
{
	constexpr glm::ivec2 healthBottomLeftOffset{ -40, -20 };
	constexpr glm::ivec2 healthTopRightOffset{ 120, 5 };
	constexpr int textHeightOffset{ 20 };
	constexpr int iconHeightOffset{ -2 };
	constexpr int iconWhitespace{ 5 };
	constexpr int secondIconWhitespace{ 2 };
	constexpr float cloudScale{ 3.0f };

	// Health goes first so the cloud is drawn on top of it
	RenderHealth(pokemon, cloudCenter + healthBottomLeftOffset, cloudCenter + healthTopRightOffset);

	// RenderCloud
	Renderer::Instance()->RenderTexture(cloud, Transform{ cloudCenter, 0, glm::vec2{ cloudScale } });

	const int textY{ cloudCenter.y + textHeightOffset };
	const int nameLeft{ cloudCenter.x + nameLeftOffset };
	const int levelRight{ cloudCenter.x + levelRightOffset };

	// Name, left aligned
	const int nameWidth{ pokemon.GetNameText()->GetTexture()->GetSize().x };
	Renderer::Instance()->RenderText(*pokemon.GetNameText(), Transform{ glm::ivec2{ nameLeft + (nameWidth / 2), textY }, 0, glm::vec2{ 1.0f } });

	// Level, right aligned
	const int levelWidth{ pokemon.GetLevelText()->GetTexture()->GetSize().x };
	Renderer::Instance()->RenderText(*pokemon.GetLevelText(), Transform{ glm::ivec2{ levelRight - (levelWidth / 2), textY }, 0, glm::vec2{ 1.0f } });

	// Element icons, right after the name
	const std::pair<Texture*, Texture*> elements{ GetElementIcons(pokemon) };
	const int iconWidth{ elements.first->GetSize().x };
	const int iconY{ textY + iconHeightOffset };

	const int firstIconX{ nameLeft + nameWidth + (iconWidth / 2) + iconWhitespace };
	Renderer::Instance()->RenderTexture(*elements.first, Transform{ glm::ivec2{ firstIconX, iconY }, 0, glm::vec2{ 1.0f } });

	if (elements.second != nullptr)
	{
		const int secondIconX{ firstIconX + iconWidth + secondIconWhitespace };
		Renderer::Instance()->RenderTexture(*elements.second, Transform{ glm::ivec2{ secondIconX, iconY }, 0, glm::vec2{ 1.0f } });
	}
}

void BattleManagerComponent::RenderHealth(const PokemonComponent& pokemon, const glm::ivec2 bottomLeft, const glm::ivec2 topRight) const
{
	constexpr Color backgroundColor{ 76, 74, 76 };

	const float healthPercentage{ pokemon.GetHealthPercentage() };
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
	const std::array<Move, 4>& moves{ m_CurrentBattle.first->GetActivePokemon()->GetMoves() };
	const glm::ivec2 elementIconSize{ m_ElementIcons.at(0)->GetSize() };
	constexpr glm::ivec2 elementIconOffset { 5.0f, -2.0f };
	constexpr glm::ivec2 arrowOffset{ -10, 0 };

	// Layout: top-left, top-right, bottom-left, bottom-right (matches m_CurrentMove indices 0-3)
	constexpr std::array<glm::ivec2, 4> movePositions
	{
		glm::ivec2{ 560, 60 },
		glm::ivec2{ 780, 60 },
		glm::ivec2{ 560, 25 },
		glm::ivec2{ 780, 25 }
	};

	for (size_t i{ 0 }; i < moves.size(); ++i)
	{
		const Move& move{ moves.at(i) };
		const glm::ivec2& position{ movePositions.at(i) };

		// Move name
		const int nameWidth{ move.Name->GetTexture()->GetSize().x };
		const glm::ivec2 namePosition{ glm::ivec2{ position.x + (nameWidth / 2), position.y } };
		Renderer::Instance()->RenderText(*move.Name, Minigin::Transform{ namePosition, 0, glm::vec2{ 1.0f } });

		// Element icon
		const glm::ivec2 elementIconPosition{ namePosition.x + (nameWidth / 2) + (elementIconSize.x / 2) + elementIconOffset.x, namePosition.y + elementIconOffset.y };
		Renderer::Instance()->RenderTexture(*m_ElementIcons.at(move.Element - 1), Minigin::Transform{ elementIconPosition, 0, glm::vec2{ 1.0f } } );
	}

	if (m_CurrentMove >= movePositions.size())
	{
		throw std::runtime_error(std::format("BattleManagerComponent::RenderMoveSelect: Invalid move index {}", m_CurrentMove));
	}

	// The move select arrow
	const glm::ivec2 arrowPosition{ movePositions.at(m_CurrentMove) + arrowOffset };
	Renderer::Instance()->RenderTexture(*m_SelectArrow, Minigin::Transform{ arrowPosition, 0, glm::vec2{ 1.0f } });
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
		wildPokemon->TakeDamage(CalculateDamage(*trainerPokemon, *wildPokemon, trainerPokemon->GetMoves().at(m_CurrentMove)));
	}
	else
	{
		trainerPokemon->TakeDamage(CalculateDamage(*wildPokemon, *trainerPokemon, wildPokemon->GetMoves().at(m_CurrentMove)));
	}
}

void BattleManagerComponent::SelectEnemyMove()
{
	const PokemonComponent& enemy{ *m_CurrentBattle.second };
	const PokemonComponent& player{ *m_CurrentBattle.first->GetActivePokemon() };
	const std::array<Move, 4>& moves{ enemy.GetMoves() };

	// Chooses random move if failed to hit smartness check and isn't a legendary pokemon
	if (m_ChanceDistribution(m_RandomEngine) < m_EnemySmartness and enemy.GetPokedexID() < POKEDEX_POKEMON_LEGENDARY_START)
	{
		std::uniform_int_distribution<int> random{ 0, 3 };
		m_CurrentMove = static_cast<uint8_t>(random(m_RandomEngine));
	}
	// Chooses a move based on scoring
	else
	{
		float bestScore{ -1.0f };
		std::vector<uint8_t> bestMoves;

		for (uint8_t moveIndex{ 0 }; moveIndex < 4; ++moveIndex)
		{
			const Move& move{ moves.at(moveIndex) };
			const float accuracy{ static_cast<float>(move.Accuracy) / 100.0f };
			const float damage{ static_cast<float>(CalculateDamage(enemy, player, move)) };

			// Scoring
			float score{ damage * accuracy };						// expected damage
			if (move.Power == 0) score = 1.0f;						// TODO: Status moves shouldn't be in the game really
			if (damage >= player.GetCurrentHealth()) score *= 2.0f; // can finish the player off

			// Checking if new best move
			if (score > bestScore + POKEMON_MOVE_SCORING_EPSILON) 
			{ 
				bestScore = score; 
				bestMoves.clear();
				bestMoves.push_back(moveIndex);
			}
			else if (std::abs(score - bestScore) <= POKEMON_MOVE_SCORING_EPSILON) 
			{ 
				bestMoves.push_back(moveIndex);
			}
		}

		std::uniform_int_distribution<size_t> pick{ 0, bestMoves.size() - 1 };
		m_CurrentMove = bestMoves[pick(m_RandomEngine)];
	}
}

bool BattleManagerComponent::CheckBattleEnd()
{
	bool battleEnded{ false }; 

	if (m_CurrentBattle.first->GetActivePokemon()->IsDead() or m_CurrentBattle.second->IsDead())
	{
		EndBattle();
		battleEnded = true;
	}

	return battleEnded;
}

void BattleManagerComponent::SetWildPokemonLevel(PokemonComponent* pokemon)
{
	pokemon->SetLevel(static_cast<uint8_t>(m_CurrentWildPokemonLevelDistribution(m_RandomEngine)));
}

bool BattleManagerComponent::MoveHits(const Move& move)
{
	return m_ChanceDistribution(m_RandomEngine) <= static_cast<float>(move.Accuracy);
}

uint16_t BattleManagerComponent::CalculateDamage(const PokemonComponent& attackingPokemon, const PokemonComponent& defendingPokemon, const Move& move) const
{
	// Raw move power
	float damage{ static_cast<float>(move.Power) };

	// Attack versus Defense depends on move if physical of special
	float statRatio{ (move.Type == MoveType::Physical) ? float(attackingPokemon.GetStats().Attack) / defendingPokemon.GetStats().Defense : float(attackingPokemon.GetStats().SpecialAttack) / defendingPokemon.GetStats().SpecialDefense };
	damage *= statRatio;

	// STAB check, if the attacking pokemon uses a move which has the same element type has the pokemon itself it receives bonus damage
	if (attackingPokemon.IsOfElement(move.Element)) damage *= 1.5f;

	// Apply element type effectivness
	damage *= m_Pokedex->GetTypeEffectiveness(move.ID, defendingPokemon.GetPokedexID());

	// Critical hit
	if (m_LastMoveCrit) damage *= m_CriticalHitMultiplier;

	return static_cast<uint16_t>(damage);
}

void BattleManagerComponent::RenderMessage(Minigin::Text& text) const
{
	const glm::ivec2 startPosition{ 550.0f, 45.0f };

	const glm::ivec2 position{ startPosition + (text.GetTexture()->GetSize() / 2) };
	Renderer::Instance()->RenderText(text, Transform{ position, 0, glm::vec2{ 1.0f } });
}

void BattleManagerComponent::PerformTurn(bool isPlayerTurn)
{
	m_IsPlayerTurn = isPlayerTurn;

	if (m_IsPlayerTurn)
	{
		m_CurrentBattleState = BattleState::PerformPlayerMove;
	}
	else
	{
		m_CurrentBattleState = BattleState::PerformEnemyMove;
		SelectEnemyMove();
	}
	
	PokemonComponent* attacker{ (m_IsPlayerTurn) ? m_CurrentBattle.first->GetActivePokemon() : m_CurrentBattle.second };
	PokemonComponent* defender{ (m_IsPlayerTurn) ? m_CurrentBattle.second : m_CurrentBattle.first->GetActivePokemon() };
	const Move& move{ attacker->GetMoves().at(m_CurrentMove) };

	m_LastMoveHit = MoveHits(move);
	m_LastMoveCrit = m_LastMoveHit and RollCritical(move);
	m_LastMoveEffectiveness = m_Pokedex->GetTypeEffectiveness(move.ID, defender->GetPokedexID());

	if (m_LastMoveHit)
	{
		UseSelectedMove();
	}
}

void BattleManagerComponent::AdvanceTurn()
{
	if (!CheckBattleEnd())
	{
		if (m_IsPlayerTurn)
		{
			PerformTurn(false);
		}
		else
		{
			m_CurrentBattleState = BattleState::SelectMove;
			m_CurrentMove = 0;
		}
	}
}

bool BattleManagerComponent::RollCritical(const Move& move)
{
	return move.Power > 0 and m_ChanceDistribution(m_RandomEngine) <= m_CriticalChance;
}

void BattleManagerComponent::LoadElementIcons()
{
	for (const PODElement& element : m_Pokedex->GetElements())
	{
		const std::filesystem::path iconPath{ std::format("Elements/{}.png", element.Name) };

		m_ElementIcons.emplace_back(Renderer::Instance()->CreateTexture(ResourceManager::Instance()->GetTextureRootPath() / iconPath));
	}
}

std::pair<Texture*, Texture*> BattleManagerComponent::GetElementIcons(const PokemonComponent& pokemon) const
{
	std::pair<uint8_t, uint8_t> pokemonElements{ pokemon.GetElements() };

	if((pokemonElements.first < 1 or pokemonElements.first > POKEDEX_ELEMENT_COUNT) or (pokemonElements.second > POKEDEX_ELEMENT_COUNT))
	{
		throw std::runtime_error{ std::format("BattleManager::GetElements(...) Invalid pokemon element id's ({}, {})", pokemonElements.first, pokemonElements.second) };
	}

	return std::make_pair(m_ElementIcons.at(pokemonElements.first - 1).get(), (pokemonElements.second > 0) ? m_ElementIcons.at(pokemonElements.second - 1).get() : nullptr);
}