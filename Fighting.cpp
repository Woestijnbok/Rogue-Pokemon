#pragma once

#include "pch.h"
#include "Fighting.h"
#include "iostream"

using namespace MyUtils;
using namespace utils;

#pragma region Constructors
Pokemon::Pokemon() :
	maxHealth{ 0.0f },
	health{ 0.0f },
	maxDefence{ 0.0f },
	defence{ 0.0f },
	maxSpecialDefence{ 0.0f },
	specialDefence{ 0.0f },
	maxSpeed{ 0.0f },
	speed{ 0.0f },
	maxAttack{ 0.0f },
	attack{ 0.0f },
	maxSpecialAttack{ 0.0f },
	specialAttack{ 0.0f },
	nameStr{ "" },
	isAlive{ false },
	name{},
	image{},
	moves{}
{
}

Pokemon::Pokemon(std::string name, float health, float defence, float speed, float attack) :

	maxHealth{ health },
	health{ maxHealth },
	maxDefence{ defence },
	defence{ maxDefence },
	maxSpecialDefence{ 0.0f },
	specialDefence{ 0.0f },
	maxSpeed{ speed },
	speed{ maxSpeed },
	maxAttack{ attack },
	attack{ maxAttack },
	maxSpecialAttack{ 0.0f },
	specialAttack{ 0.0f },
	nameStr{ name },
	isAlive{ true },
	name{},
	image{},
	moves{}
{
}

Attack::Attack() :
	nameStr{ "" },
	attack{ 0.0f },
	specialAttack{ 0.0f },
	upSpeed{ 0.0f },
	upAttack{ 0.0f },
	upSpecialAttack{ 0.0f },
	upDefence{ 0.0f },
	upSpecialDefence{ 0.0f },
	name{},
	attackEffect{},
	elapsedSec{ 0.0f },
	isDynamic{ true }
{
}

Attack::Attack(std::string name, float attack):
	nameStr{ name },
	attack{ attack },
	specialAttack{ 0.0f },
	upSpeed{ 0.0f },
	upAttack{ 0.0f },
	upSpecialAttack{ 0.0f },
	upDefence{ 0.0f },
	upSpecialDefence{ 0.0f },
	name{},
	attackEffect{},
	elapsedSec{ 0.0f },
	isDynamic{true}
{
}

Attack::Attack(std::string name, float attack, bool dynamic) :
	nameStr{ name },
	attack{ attack },
	specialAttack{ 0.0f },
	upSpeed{ 0.0f },
	upAttack{ 0.0f },
	upSpecialAttack{ 0.0f },
	upDefence{ 0.0f },
	upSpecialDefence{ 0.0f },
	name{},
	attackEffect{},
	elapsedSec{ 0.0f },
	isDynamic{dynamic}
{
}
#pragma endregion Constructors

#pragma region Functions
void SetAttacks()
{
	//Charizard
	g_AlliedPokemon.moves[0] = g_HyperBeam;
	g_AlliedPokemon.moves[1] = g_Tornado;
	g_AlliedPokemon.moves[2] = g_Bite;
	g_AlliedPokemon.moves[3] = g_Flamethrower;

	// --Legendary Pokemon--

	//Mew
	g_ArrLegendaryPokemons[0].moves[0] = g_MegaPunch;
	g_ArrLegendaryPokemons[0].moves[1] = g_Psychic;
	g_ArrLegendaryPokemons[0].moves[2] = g_Ancientpower;
	g_ArrLegendaryPokemons[0].moves[3] = g_AuraSphere;

	//Zapdos
	g_ArrLegendaryPokemons[1].moves[0] = g_Ancientpower;
	g_ArrLegendaryPokemons[1].moves[1] = g_Discharge;
	g_ArrLegendaryPokemons[1].moves[2] = g_DrillPeck;
	g_ArrLegendaryPokemons[1].moves[3] = g_Thunder;

	//Deoxys
	g_ArrLegendaryPokemons[2].moves[0] = g_Psychic;
	g_ArrLegendaryPokemons[2].moves[1] = g_ZenHeadbutt;
	g_ArrLegendaryPokemons[2].moves[2] = g_PsychoBoost;
	g_ArrLegendaryPokemons[2].moves[3] = g_HyperBeam;

	// --Enemy Pokemon--

	//Pidgey
	g_ArrEnemyPokemons[0].moves[0] = g_Tackle;
	g_ArrEnemyPokemons[0].moves[1] = g_Growl;
	g_ArrEnemyPokemons[0].moves[2] = g_Bite;
	g_ArrEnemyPokemons[0].moves[3] = g_Tornado;

	//Rattata
	g_ArrEnemyPokemons[1].moves[0] = g_Tackle;
	g_ArrEnemyPokemons[1].moves[1] = g_Bite;
	g_ArrEnemyPokemons[1].moves[2] = g_Tail;
	g_ArrEnemyPokemons[1].moves[3] = g_Quick;

	//Sandshrew
	g_ArrEnemyPokemons[2].moves[0] = g_Tackle;
	g_ArrEnemyPokemons[2].moves[1] = g_Sand;
	g_ArrEnemyPokemons[2].moves[2] = g_Bite;
	g_ArrEnemyPokemons[2].moves[3] = g_Slash;

	//Geodude
	g_ArrEnemyPokemons[3].moves[0] = g_Tackle;
	g_ArrEnemyPokemons[3].moves[1] = g_Bite;
	g_ArrEnemyPokemons[3].moves[2] = g_Sand;
	g_ArrEnemyPokemons[3].moves[3] = g_Rock;

	//Gengar
	g_ArrEnemyPokemons[4].moves[0] = g_Tackle;
	g_ArrEnemyPokemons[4].moves[1] = g_Bite;
	g_ArrEnemyPokemons[4].moves[2] = g_Hex;
	g_ArrEnemyPokemons[4].moves[3] = g_Shadowball;
}

void InitializeAttacks()
{
	const Color4f black(0, 0, 0, 1);

	// allies
	for (int j{}; j < g_MaxNrMoves; ++j)
	{
		TextureFromString(g_AlliedPokemon.moves[j].nameStr, "Resources/FireRed Font.ttf", g_TextSize, black, g_AlliedPokemon.moves[j].name);
		if (!utils::TextureFromFile("resources/fighting/" + g_AlliedPokemon.moves[j].nameStr + ".png", g_AlliedPokemon.moves[j].attackEffect))
		{
			std::cout<< "allies" << j << ": attacks texture failed\n";
		}
	}

	//legendary
	for (int i{}; i < g_NrLegendaryPokemons; ++i)
	{
		for (int j{}; j < g_MaxNrMoves; ++j)
		{
			if (!TextureFromString(g_ArrLegendaryPokemons[i].moves[j].nameStr, "Resources/FireRed Font.ttf", g_TextSize, black, g_ArrLegendaryPokemons[i].moves[j].name))
			{
				std::cout << i << j << " legendary pokemon not loaded" << std::endl;
			}
			if (!utils::TextureFromFile("resources/fighting/" + g_ArrLegendaryPokemons[i].moves[j].nameStr + ".png", g_ArrLegendaryPokemons[i].moves[j].attackEffect))
			{
				std::cout << "legends" << j << ": attacks files failed\n";
			}
		}
	}

	//enemies
	for (int indexPokemon{}; indexPokemon < g_NrEnemyPokemons; ++indexPokemon)
	{
		for (int indexAttacks{}; indexAttacks < g_MaxNrMoves; ++indexAttacks)
		{
			if (!TextureFromString(g_ArrEnemyPokemons[indexPokemon].moves[indexAttacks].nameStr, "Resources/FireRed Font.ttf", g_TextSize, black, g_ArrEnemyPokemons[indexPokemon].moves[indexAttacks].name))
			{
				std::cout << indexPokemon << " enemy pokemon not loaded" << std::endl;
			}
			if (!utils::TextureFromFile("resources/fighting/" + g_ArrEnemyPokemons[indexPokemon].moves[indexAttacks].nameStr + ".png", g_ArrEnemyPokemons[indexPokemon].moves[indexAttacks].attackEffect))
			{
				std::cout << indexPokemon << ": attacks texture failed\n";
			}
		}
	}
}

void InitializePokemons()
{
	const Color4f black(0, 0, 0, 1);

	TextureFromString(g_AlliedPokemon.nameStr, "Resources/FireRed Font.ttf", g_NameSize, black, g_AlliedPokemon.name);
	TextureFromFile("Resources/Fighting/" + g_AlliedPokemon.nameStr + ".png", g_AlliedPokemon.image);

	for (int i{}; i < g_NrLegendaryPokemons; ++i)
	{
		TextureFromString(g_ArrLegendaryPokemons[i].nameStr, "Resources/FireRed Font.ttf", g_NameSize, black, g_ArrLegendaryPokemons[i].name);
		TextureFromFile("Resources/Fighting/" + g_ArrLegendaryPokemons[i].nameStr + ".png", g_ArrLegendaryPokemons[i].image);
	}

	for (int i{}; i < g_NrEnemyPokemons; ++i)
	{
		TextureFromString(g_ArrEnemyPokemons[i].nameStr, "Resources/FireRed Font.ttf", g_NameSize, black, g_ArrEnemyPokemons[i].name);
		TextureFromFile("Resources/Fighting/" + g_ArrEnemyPokemons[i].nameStr + ".png", g_ArrEnemyPokemons[i].image);
	}
}

void DeleteAttacks()
{
	DeleteTexture(g_Tackle.name);//
	DeleteTexture(g_Tackle.attackEffect);
	DeleteTexture(g_Bite.name);//
	DeleteTexture(g_Bite.attackEffect);
	DeleteTexture(g_Flamethrower.name);//
	DeleteTexture(g_Flamethrower.attackEffect);
	DeleteTexture(g_Hex.name);//
	DeleteTexture(g_Hex.attackEffect);
	DeleteTexture(g_Shadowball.name);//
	DeleteTexture(g_Shadowball.attackEffect);
	DeleteTexture(g_Growl.name);
	DeleteTexture(g_Tail.name);
	DeleteTexture(g_Quick.name);//
	DeleteTexture(g_Quick.attackEffect);
	DeleteTexture(g_Sand.name);//
	DeleteTexture(g_Sand.attackEffect);
	DeleteTexture(g_Rock.name);//
	DeleteTexture(g_Rock.attackEffect);
	DeleteTexture(g_Tornado.name);//
	DeleteTexture(g_Tornado.attackEffect);
	DeleteTexture(g_Slash.name);//
	DeleteTexture(g_Slash.attackEffect);
	DeleteTexture(g_Surf.name);//
	DeleteTexture(g_Surf.attackEffect);
	DeleteTexture(g_PsychoBoost.name);
	DeleteTexture(g_PsychoBoost.attackEffect);
	DeleteTexture(g_ZenHeadbutt.name);
	DeleteTexture(g_ZenHeadbutt.attackEffect);
	DeleteTexture(g_HyperBeam.name);
	DeleteTexture(g_HyperBeam.attackEffect);
	DeleteTexture(g_Psychic.name);//
	DeleteTexture(g_Psychic.attackEffect);
	DeleteTexture(g_MegaPunch.name);//
	DeleteTexture(g_MegaPunch.attackEffect);
	DeleteTexture(g_Ancientpower.name);
	DeleteTexture(g_Ancientpower.attackEffect);
	DeleteTexture(g_AuraSphere.name);
	DeleteTexture(g_AuraSphere.attackEffect);
	DeleteTexture(g_Discharge.name);
	DeleteTexture(g_Discharge.attackEffect);
	DeleteTexture(g_DrillPeck.name);//
	DeleteTexture(g_DrillPeck.attackEffect);
	DeleteTexture(g_Thunder.name);//
	DeleteTexture(g_Thunder.attackEffect);



}

void DeletePokemons()
{
	DeleteTexture(g_AlliedPokemon.name);
	DeleteTexture(g_AlliedPokemon.image);
	for (int attackIndex{}; attackIndex < g_MaxNrMoves; ++attackIndex)
	{
		utils::DeleteTexture(g_AlliedPokemon.moves[attackIndex].name);
		utils::DeleteTexture(g_AlliedPokemon.moves[attackIndex].attackEffect);
	}

	for (int i{}; i < g_NrLegendaryPokemons; ++i)
	{
		DeleteTexture(g_ArrLegendaryPokemons[i].name);
		DeleteTexture(g_ArrLegendaryPokemons[i].image);
		for (int attackIndex{}; attackIndex < g_MaxNrMoves; ++attackIndex)
		{
			utils::DeleteTexture(g_ArrLegendaryPokemons[i].moves[attackIndex].name);
			utils::DeleteTexture(g_ArrLegendaryPokemons[i].moves[attackIndex].attackEffect);
		}
	}

	for (int i{}; i < g_NrEnemyPokemons; ++i)
	{
		DeleteTexture(g_ArrEnemyPokemons[i].name);
		DeleteTexture(g_ArrEnemyPokemons[i].image);
		for (int attackIndex{}; attackIndex < g_MaxNrMoves; ++attackIndex)
		{
			utils::DeleteTexture(g_ArrEnemyPokemons[i].moves[attackIndex].name);
			utils::DeleteTexture(g_ArrEnemyPokemons[i].moves[attackIndex].attackEffect);
		}
	}
}

void InitializeBattle()
{
	const Color4f black{ 0.0f, 0.0f, 0.0f, 1.0f };

	TextureFromFile("Resources/Fighting/Battle Backround.png", g_BattleBackround);
	TextureFromFile("Resources/Fighting/Info Box.png", g_InfoBox);
	TextureFromFile("Resources/Fighting/Select Move Box.png", g_SelectMoveBox);
	TextureFromString("appeared!", "Resources/FireRed Font.ttf", g_TextSize, black, g_Appeared);
	TextureFromString("used", "Resources/FireRed Font.ttf", g_TextSize, black, g_Used);
	TextureFromString("fainted!", "Resources/FireRed Font.ttf", g_TextSize, black, g_Fainted);
	TextureFromFile("Resources/Fighting/Arrow.png", g_Arrow);
	TextureFromFile("Resources/Fighting/Your Cloud.png", g_YourCloud);
	TextureFromFile("Resources/Fighting/Enemy Cloud.png", g_EnemyCloud);
}

void DeleteBattle()
{
	DeleteTexture(g_BattleBackround);
	DeleteTexture(g_InfoBox);
	DeleteTexture(g_SelectMoveBox);
	DeleteTexture(g_Appeared);
	DeleteTexture(g_Used);
	DeleteTexture(g_Fainted);
	DeleteTexture(g_Arrow);
	DeleteTexture(g_EnemyCloud);
	DeleteTexture(g_YourCloud);
}

void SelectMove(SDL_Keycode key)
{
	if(g_FightSequence == FightSequence::selectMove)
	{
		switch (key)
		{
			case SDLK_UP:
				switch (g_YourChoice)
				{
					case MoveSelecter::leftDown:
						g_YourChoice = MoveSelecter::leftUp;
						break;
					case MoveSelecter::rightDown:
						g_YourChoice = MoveSelecter::rightUp;
						break;
				}
				break;
			case SDLK_DOWN:
				switch (g_YourChoice)
				{
					case MoveSelecter::rightUp:
						g_YourChoice = MoveSelecter::rightDown;
						break;
					case MoveSelecter::leftUp:
						g_YourChoice = MoveSelecter::leftDown;
						break;
				}
				break;
			case SDLK_RIGHT:
				switch (g_YourChoice)
				{
					case MoveSelecter::leftUp:
						g_YourChoice = MoveSelecter::rightUp;
						break;
					case MoveSelecter::leftDown:
						g_YourChoice = MoveSelecter::rightDown;
						break;
				}
				break;
			case SDLK_LEFT:
				switch (g_YourChoice)
				{
					case MoveSelecter::rightUp:
						g_YourChoice = MoveSelecter::leftUp;
						break;
					case MoveSelecter::rightDown:
						g_YourChoice = MoveSelecter::leftDown;
						break;
				}
				break;
		}
	}
}

void ProceedBattle(SDL_Keycode key)
{
	if(key == SDLK_SPACE)
	{
		switch (g_FightSequence)
		{
		case FightSequence::start:
			g_FightSequence = FightSequence::selectMove;
			break;
		case FightSequence::selectMove:
			g_FightSequence = FightSequence::yourText;
			break;
		case FightSequence::yourText:
			g_FightSequence = FightSequence::yourMove;
			break;
		case FightSequence::enemyText:
			g_FightSequence = FightSequence::enemyMove;
			break;
		case FightSequence::winner:
			g_FightSequence = FightSequence::end;
			break;
		}
	}
}

void ForceEndBattle(SDL_Keycode key)
{
	if (key == SDLK_f)
	{
		g_FightSequence = FightSequence::end;
		g_pEnemyPokemon->health = 0.0f;
		g_pEnemyPokemon->isAlive = false;
	}
}

void RegisterMove(float time)
{
	
		switch (g_FightSequence)
		{
			case FightSequence::yourMove:
				g_AlliedPokemon.moves[int(g_YourChoice)].elapsedSec += time;
				if (g_AlliedPokemon.moves[int(g_YourChoice)].elapsedSec + time >= 1.5f)
				{
					g_pEnemyPokemon->health -= g_AlliedPokemon.moves[int(g_YourChoice)].attack;
					g_AlliedPokemon.moves[int(g_YourChoice)].elapsedSec = 0;
					if (g_pEnemyPokemon->health <= 0.0f)
					{
						g_pEnemyPokemon->health = 0.0f;
						g_pEnemyPokemon->isAlive = false;
						g_FightSequence = FightSequence::winner;
					}
					else g_FightSequence = FightSequence::enemyChoose;
				}
					break;
			case FightSequence::enemyMove:
				g_pEnemyPokemon->moves[int(g_EnemyChoice)].elapsedSec += time;
				if (g_pEnemyPokemon->moves[int(g_EnemyChoice)].elapsedSec + time >= 1.5f)
				{
					g_pEnemyPokemon->moves[int(g_EnemyChoice)].elapsedSec = 0;
					g_AlliedPokemon.health -= g_pEnemyPokemon->moves[int(g_EnemyChoice)].attack;
					if (g_AlliedPokemon.health <= 0.0f)
					{
						g_AlliedPokemon.health = 0.0f;
						g_AlliedPokemon.isAlive = false;
						g_FightSequence = FightSequence::winner;
					}
					else g_FightSequence = FightSequence::selectMove;
				}
					break;
		}
}

void NewEnemyMove()
{
	const int number{GetRandomNumber(0, 3)};

	switch (number)
	{
		case 0:
			g_EnemyChoice = MoveSelecter::leftDown;
			break;
		case 1:
			g_EnemyChoice = MoveSelecter::rightDown;
			break;
		case 2:
			g_EnemyChoice = MoveSelecter::rightUp;
			break;
		case 3:
			g_EnemyChoice = MoveSelecter::leftUp;
			break;
	}
}

void UpdateBattle(float time)
{
	switch (g_FightSequence)
	{
		case FightSequence::enemyChoose:
			NewEnemyMove();
			g_FightSequence = FightSequence::enemyText;
			break;
		case FightSequence::yourMove:
		case FightSequence::enemyMove:
			RegisterMove(time);
			break;
	}
}

void NewEnemy()
{
	if (GetChance(g_ChanceLegendaryEncounter))
	{
		do
		{
			g_pEnemyPokemon = &g_ArrLegendaryPokemons[GetRandomNumber(0, g_NrLegendaryPokemons - 1)];
		} while (!g_pEnemyPokemon->isAlive);
	}
	else
	{
		do
		{
			g_pEnemyPokemon = &g_ArrEnemyPokemons[GetRandomNumber(0, g_NrEnemyPokemons - 1)];
		} while (!g_pEnemyPokemon->isAlive);
	}
}

void ResetEnemies()
{
	for (int index{}; index < g_NrEnemyPokemons; ++index)
	{
		if (!g_ArrEnemyPokemons[index].isAlive)
		{
			g_ArrEnemyPokemons[index].isAlive = true;
			g_ArrEnemyPokemons[index].health = g_ArrEnemyPokemons[index].maxHealth;
		}
	}

	for (int index{}; index < g_NrLegendaryPokemons; ++index)
	{
		if (!g_ArrLegendaryPokemons[index].isAlive)
		{
			g_ArrLegendaryPokemons[index].isAlive = true;
			g_ArrLegendaryPokemons[index].health = g_ArrLegendaryPokemons[index].maxHealth;
		}
	}
}

void DrawSelectMoves()
{
	const float borderY{ 20.0f };
	const float borderX{ 100.0f };

	// Left down move which is move 0.

	Point2f position{g_RectfBattleBox.left + borderX, g_RectfBattleBox.bottom + borderY };
	DrawTexture(g_AlliedPokemon.moves[0].name, position);
	if (g_YourChoice == MoveSelecter::leftDown) DrawTexture(g_Arrow, Point2f{ position.x - 10.0f, position.y + 5.0f });

	// Right down move which is move 1.

	position = Point2f{ g_RectfBattleBox.left + g_RectfBattleBox.width - borderX - g_AlliedPokemon.moves[3].name.width, g_RectfBattleBox.bottom + borderY };
	DrawTexture(g_AlliedPokemon.moves[1].name, position);
	if (g_YourChoice == MoveSelecter::rightDown) DrawTexture(g_Arrow, Point2f{ position.x - 10.0f, position.y + 5.0f });

	// Left up move which is move 2.

	position = Point2f{ g_RectfBattleBox.left + borderX, g_RectfBattleBox.bottom + g_RectfBattleBox.height - borderY - g_AlliedPokemon.moves[2].name .height};
	DrawTexture(g_AlliedPokemon.moves[2].name, position);
	if (g_YourChoice == MoveSelecter::leftUp) DrawTexture(g_Arrow, Point2f{ position.x - 10.0f, position.y + 5.0f });
	
	// Right up move which is move 3.

	position = Point2f{ g_RectfBattleBox.left + g_RectfBattleBox.width - borderX - g_AlliedPokemon.moves[3].name.width,
		g_RectfBattleBox.bottom + g_RectfBattleBox.height - borderY - g_AlliedPokemon.moves[3].name.height };
	DrawTexture(g_AlliedPokemon.moves[3].name, position);
	if (g_YourChoice == MoveSelecter::rightUp) DrawTexture(g_Arrow, Point2f{ position.x - 10.0f, position.y + 5.0f });
}

void DrawInfo()
{
	switch (g_FightSequence)
	{
		case FightSequence::start:
			DrawTexture(g_pEnemyPokemon->name,
				Point2f{ g_RectfBattleBox.left + 30.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 10.0f });
			DrawTexture(g_Appeared,
				Point2f{ g_RectfBattleBox.left + g_pEnemyPokemon->name.width + 40.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 7.0f });
			break;
		case FightSequence::yourText:
			DrawTexture(g_AlliedPokemon.name,
				Point2f{ g_RectfBattleBox.left + 30.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 10.0f });
			DrawTexture(g_Used,
				Point2f{ g_RectfBattleBox.left + g_AlliedPokemon.name.width + 40.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 7.0f });
			DrawTexture(g_AlliedPokemon.moves[int(g_YourChoice)].name,
				Point2f{ g_RectfBattleBox.left + g_AlliedPokemon.name.width + g_Used.width + 50.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 7.0f });
			break;
		case FightSequence::enemyText:
			DrawTexture(g_pEnemyPokemon->name,
				Point2f{ g_RectfBattleBox.left + 30.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 10.0f });
			DrawTexture(g_Used,
				Point2f{ g_RectfBattleBox.left + g_pEnemyPokemon->name.width + 40.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 7.0f });
			DrawTexture(g_pEnemyPokemon->moves[int(g_EnemyChoice)].name,
				Point2f{ g_RectfBattleBox.left + g_pEnemyPokemon->name.width + g_Used.width + 50.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 7.0f });
			break;
		case FightSequence::winner:
			if (!g_AlliedPokemon.isAlive)
			{
				DrawTexture(g_AlliedPokemon.name,
					Point2f{ g_RectfBattleBox.left + 30.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 10.0f });
				DrawTexture(g_Fainted,
					Point2f{ g_RectfBattleBox.left + g_AlliedPokemon.name.width + 40.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 7.0f });
			}
			else
			{
				DrawTexture(g_pEnemyPokemon->name,
					Point2f{ g_RectfBattleBox.left + 30.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 10.0f });
				DrawTexture(g_Fainted,
					Point2f{ g_RectfBattleBox.left + g_pEnemyPokemon->name.width + 40.0f, g_RectfBattleBox.bottom + (g_RectfBattleBox.height / 2.0f) + (g_TextSize / 2.0f) - 7.0f });
			}
			break;
	}
}

void DrawBattleBox()
{
	

	switch (g_FightSequence)
	{
		case FightSequence::selectMove:
			DrawTexture(g_SelectMoveBox, g_RectfBattleBox);
			DrawSelectMoves();
			break;
		case FightSequence::start:
		case FightSequence::yourText:
		case FightSequence::enemyText:
		case FightSequence::winner:
			DrawTexture(g_InfoBox, g_RectfBattleBox);
			DrawInfo();
			break;
	}
}

void DrawHealth()
{
	const float maxWidth{ 140.0f };

	const float healthPercentageAlly{ g_AlliedPokemon.health / g_AlliedPokemon.maxHealth };
	const float widthAlly{ maxWidth * healthPercentageAlly };

	if (healthPercentageAlly <= 0.2f) SetColor(1.0f, 0.0f, 0.0f);
	else if (healthPercentageAlly <= 0.5f) SetColor(1.0f, 0.64f, 0.0f);
	else if (healthPercentageAlly >= 0.5f) SetColor(0.0f, 1.0f, 0.0f);

	const Rectf yourHealthBar{g_RectfYourCloud.left + 117.0f, g_RectfYourCloud.bottom+ 26.0f, widthAlly, 10.0f };
	FillRect(yourHealthBar);

	const float healthPercentageEnemy = g_pEnemyPokemon->health / g_pEnemyPokemon->maxHealth ;
	const float widthEnemy = maxWidth * healthPercentageEnemy;

	if (healthPercentageEnemy <= 0.2f) SetColor(1.0f, 0.0f, 0.0f);
	else if (healthPercentageEnemy <= 0.5f) 
	{
		SetColor(1.0f, 0.64f, 0.0f);
	}
	else if (healthPercentageEnemy >= 0.5f) SetColor(0.0f, 1.0f, 0.0f);

	const Rectf enemyHealthBar{ g_RectfEnemyCloud.left + 117.0f, g_RectfEnemyCloud.bottom + 26.0f, widthEnemy, 10.0f };
	FillRect(enemyHealthBar);
}

void DrawBattle()
{
	const int centerOffset{ 50 };
	const int border{ 5 };
	Point2f posImg1(border, border);
	Point2f posImg2(g_WindowWidth / 2, g_WindowHeight / 2);
	Point2f posPokemonHitSpot1(posImg1.x + centerOffset, posImg1.y + centerOffset);
	Point2f posPokemonHitSpot2(posImg2.x + centerOffset, posImg2.y + centerOffset);

	DrawTexture(g_BattleBackround, Point2f{ 0.0f, 0.0f });
	DrawTexture(g_AlliedPokemon.image, g_RectfAlliedPokemon);
	DrawTexture(g_pEnemyPokemon->image, g_RectfEnemyPokemon);
	DrawTexture(g_YourCloud, g_RectfYourCloud);
	DrawTexture(g_EnemyCloud, g_RectfEnemyCloud);
	DrawTexture(g_AlliedPokemon.name, Point2f{ g_RectfYourCloud.left + 40.0f, g_RectfYourCloud.bottom + 45.0f});
	DrawTexture(g_pEnemyPokemon->name, Point2f{ g_RectfEnemyCloud.left + 20.0f, g_RectfEnemyCloud.bottom + 45.0f });
	DrawHealth();
	switch (g_FightSequence)
	{
		case FightSequence::yourMove:
			DrawFightAnimation(g_AlliedPokemon, g_YourChoice, posPokemonHitSpot1, posPokemonHitSpot2);
			break;

		case FightSequence::enemyMove:
			DrawFightAnimation(*g_pEnemyPokemon, g_EnemyChoice, posPokemonHitSpot2, posPokemonHitSpot1);
			break;
	}
	DrawBattleBox();
}

void HealParty()
{
	g_AlliedPokemon.health = g_AlliedPokemon.maxHealth;
	g_AlliedPokemon.isAlive = true;
}

void DrawFightAnimation(const Pokemon& pokemon, const MoveSelecter& attackIndex, const Point2f& start, const Point2f& end)
{
	Vector2f vector{ end.x - start.x, end.y - start.y };
	float width{ pokemon.moves[int(attackIndex)].attackEffect.width };
	float height{ pokemon.moves[int(attackIndex)].attackEffect.height };
	float time{ pokemon.moves[int(attackIndex)].elapsedSec };
	Rectf projectile(start.x + vector.x * (time * 1.5f), start.y + vector.y * (time * 1.5f), width, height);

	if (pokemon.moves[int(attackIndex)].isDynamic)
	{
		utils::DrawTexture(pokemon.moves[int(attackIndex)].attackEffect, projectile);
	}
	else
	{
		const Rectf centerScreen(0, 0, g_WindowWidth, g_WindowHeight);
		utils::DrawTexture(pokemon.moves[int(attackIndex)].attackEffect, centerScreen);
	}

	//draws red growing circle to show being hit
	if (pokemon.moves[int(attackIndex)].elapsedSec >= 1)
	{
		float radius{ 10 * powf(pokemon.moves[int(attackIndex)].elapsedSec, 10) };
		float opacity{ 0.75f / powf(pokemon.moves[int(attackIndex)].elapsedSec, 8) };
		utils::SetColor(1.0f, 0.0f, 0.0f, opacity);
		utils::FillEllipse(end.x, end.y, radius, radius);
	}
}
#pragma endregion Functions