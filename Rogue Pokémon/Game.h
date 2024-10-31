#pragma once

#include "Exploring.h"
#include "Fighting.h"

#pragma region gameInformation
std::string g_WindowTitle{ "Scuffed Pokémon - Made By Gommers, Tijl & Demeyer, Artuur - 1DAE11" };
float g_WindowWidth{ 1280.0f };
float g_WindowHeight{ 720.0f };
#pragma endregion gameInformation

#pragma region ExploringVariables
Texture g_Grass{};

Texture g_Dirt{};

Sprite g_Right{};

Sprite g_Left{};

Sprite g_Up{};

Sprite g_Down{};

Facing g_PlayerFacing{ Facing::down };

Movement g_PlayerMovement{ Movement::still };

const float g_GridSize{ 80.0f };

const int g_NrRows{ int(g_WindowHeight / g_GridSize) };

const int g_NrColls{ int(g_WindowWidth / g_GridSize) };

Tile* g_pArrMap{ new Tile[g_NrRows * g_NrColls]{} };

const float g_GrassChance{ 25.0f };

const float g_PokemonChance{ 25.0f };

Circlef g_Player{};

float g_MovementTimer{};

int g_NrDefeatedenemies{};

float g_PokemonIdleTime{};

float g_PokemonMaxIdleTime{ 1.0f };

const float g_ItemChance { 1.0f };

float g_ChanceLegendaryEncounter{ 1.0f };

extern Texture g_Pokeball{};

int g_Score{};
#pragma endregion ExploringVariables

#pragma region FightZoneVariables
Texture g_Healed{};

const float g_ScalePokemonImage{ 5.0f };

const Rectf g_RectfAlliedPokemon{ 150.0f, 0.0f, 64.0f * g_ScalePokemonImage, 64.0f * g_ScalePokemonImage };

const Rectf g_RectfEnemyPokemon{ 760.0f, 300.0f, 64.0f * g_ScalePokemonImage, 64.0f * g_ScalePokemonImage };

const float g_ScaleBattleBox{ 2.5f };

const Rectf g_RectfBattleBox{675.0f, 5.0f, 240.0f * g_ScaleBattleBox, 48.0f * g_ScaleBattleBox };

Texture g_BattleBackround{};

Texture g_InfoBox{};

Texture g_SelectMoveBox{};

const int g_TextSize{ 30 };

Texture g_Appeared{};

Texture g_Used{};

Texture g_Fainted{};

const int g_NameSize{ 45 };

Texture g_Arrow{};

const float g_ScaleCloud{ 3.0f };

const Rectf g_RectfYourCloud{ 680.0f, 140.0f, 100.0f * g_ScaleCloud, 29.0f * g_ScaleCloud };

const Rectf g_RectfEnemyCloud{ 400.0f, 550.0f, 100.0f * g_ScaleCloud, 29.0f * g_ScaleCloud };

Texture g_YourCloud{};

Texture g_EnemyCloud{};

const int g_MaxNrMoves{ 4 };

MoveSelecter g_YourChoice{};

MoveSelecter g_EnemyChoice{};
	
FightSequence g_FightSequence{ FightSequence::start };

Pokemon* g_pEnemyPokemon{ nullptr };

Pokemon g_AlliedPokemon{ Pokemon("Charizard", 120, 50, 50, 100) };

const int g_NrLegendaryPokemons{ 3 };

Pokemon g_ArrLegendaryPokemons[g_NrLegendaryPokemons]
{
	Pokemon("Mew", 100, 100, 100, 100),
	Pokemon("Zapdos", 90, 85, 100, 90),
	Pokemon("Deoxys", 50, 50, 150, 150)
};

const int g_NrEnemyPokemons{ 5 };

Pokemon g_ArrEnemyPokemons[g_NrEnemyPokemons]
{
	Pokemon("Pidgey", 60, 50, 50, 100),
	Pokemon("Ratata", 60, 70, 60, 90),
	Pokemon("Sandshrew", 70, 50, 50, 100),
	Pokemon("Geodude", 70, 70, 60, 90),
	Pokemon("Gengar", 100, 70, 60, 90)
};
#pragma region Attacks
	Attack g_Tackle("Tackle", 45);

	Attack g_Bite("Bite", 30);

	Attack g_Flamethrower("Fire Ball", 60);

	Attack g_Shadowball("Shadow Ball", 60);

	Attack g_Hex("Hex", 60, false);

	Attack g_Growl("Growl", 0);

	Attack g_Tail("TailWhip", 0);

	Attack g_Quick("Quick Attack", 40);

	Attack g_Rock("Rock throw", 50);

	Attack g_Sand("Sand attack", 20);

	Attack g_PsychoBoost("Psycho Boost", 140);

	Attack g_HyperBeam("Hyper Beam", 150);

	Attack g_ZenHeadbutt("Zen Headbutt", 80);

	Attack g_Psychic("Psychic", 90, false);

	Attack g_MegaPunch("Mega Punch", 80, false);

	Attack g_Ancientpower("Ancientpower", 60);

	Attack g_AuraSphere("Aura Sphere", 90);

	Attack g_Discharge("Discharge", 80);

	Attack g_DrillPeck("Drill Peck", 80);

	Attack g_Thunder("Thunder", 120, false);

	Attack g_Tornado("Tornado", 50);

	Attack g_Slash("Slash", 20);

	Attack g_Surf("Surf", 45);
	#pragma endregion Attacks
#pragma endregion FightZoneVariables

#pragma region Enumerations
enum class GameState
{
	exploring,
	fighting,
	pickup,
};
#pragma endregion Enumerations

#pragma region Variables
GameState g_Gamestate{ GameState::exploring };

#pragma endregion Variables

#pragma region Functions
void CheckTile();

void CheckBattle();

void AcceptItem(SDL_Keycode key);

void CheckNewMap();

void PrintInformation(SDL_Keycode key);
#pragma endregion Functions

#pragma region gameFunctions											
void Start();

void Draw();

void Update(float elapsedSec);

void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);

void OnKeyUpEvent(SDL_Keycode key);

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);

void OnMouseDownEvent(const SDL_MouseButtonEvent& e);

void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
