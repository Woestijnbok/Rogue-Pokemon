#pragma once

#include "pch.h"
#include "Game.h"
#include "iostream"

using namespace utils;
using namespace MyUtils;

#pragma region gameFunctions											
void Start()
{
	//world------------------------------
	LoadTexturesExploring();
	SetSeedMap();
	SetPlayer();

	//fight--------------------------------
	SetAttacks();
	InitializeAttacks();
	InitializePokemons();
	InitializeBattle();
}

void Draw()
{
	ClearBackground();
	switch (g_Gamestate)
	{
		case GameState::pickup:
			TexturizeMap();
			DisplayPickup();
			TexturizePlayer();
			break;
		case GameState::exploring:
			TexturizeMap();
			//DrawMap();
			TexturizePlayer();
			//DrawPlayer();
			break;
		case GameState::fighting:
			DrawBattle();
			break;
	}
}

void Update(float elapsedSec)
{
	switch (g_Gamestate)
	{
		case GameState::exploring:
			CheckNewMap();
			MovePokemons(elapsedSec);
			UpdatePlayer(elapsedSec);
			UpdateTexturePlayer(elapsedSec);
			CheckTile();
			break;
		case GameState::fighting:
			UpdateBattle(elapsedSec);
			CheckBattle();
			break;
	}
}

void End()
{
	//world------------------------------
	DeleteTexturesExploring();

	//fight--------------------------------
	DeleteAttacks();
	DeletePokemons();
	DeleteBattle();
}
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
	switch (g_Gamestate)
	{
		case GameState::exploring:
			MovePlayer(key);
			break;
	}
}

void OnKeyUpEvent(SDL_Keycode key)
{
	PrintInformation(key);

	switch (g_Gamestate)
	{
		case GameState::fighting:
			ProceedBattle(key);
			SelectMove(key);
			ForceEndBattle(key);
			break;
		case GameState::pickup:
			AcceptItem(key);
	}
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	
}
#pragma endregion inputHandling

#pragma region ownDefinitions
void CheckTile()
{
	switch (g_pArrMap[GetIndexPlayerTile()])
	{
		case Tile::pokemon:
			NewEnemy();
			g_FightSequence = FightSequence::start;
			g_Gamestate = GameState::fighting;
			break;
		case Tile::item:
			g_Gamestate = GameState::pickup;
			HealParty();
			g_pArrMap[GetIndexPlayerTile()] = Tile::dirt;
			break;
	}
}

void CheckBattle()
{
	if (g_FightSequence == FightSequence::end)
	{
		g_pArrMap[GetIndexPlayerTile()] = Tile::grass;

		if (!g_pEnemyPokemon->isAlive)
		{
			++g_NrDefeatedenemies;
			++g_Score;
			++g_ChanceLegendaryEncounter;
			g_Gamestate = GameState::exploring;
		}
		else
		{
			SetSeedMap();
			SetPlayer();
			ResetEnemies();
			HealParty();
			g_NrDefeatedenemies = 0;
			g_Score = 0;
			g_ChanceLegendaryEncounter = 1.0f;
			g_Gamestate = GameState::exploring;
		}

		g_pEnemyPokemon = nullptr;
		std::cout << "Your current score: " << g_Score << std::endl;
	}
}

void AcceptItem(SDL_Keycode key)
{
	if (key == SDLK_SPACE)
	{
		g_Gamestate = GameState::exploring;
	}
}

void CheckNewMap()
{
	if (g_NrDefeatedenemies == 3)
	{
		SetSeedMap();
		SetPlayer();
		ResetEnemies();
		g_NrDefeatedenemies = 0;
	}
}

void PrintInformation(SDL_Keycode key)
{
	if (key == SDLK_i)
	{
		std::cout << "-- Game controls --" << std::endl;
		std::cout << "'left', 'rigth', 'up' and 'down' are used for movement and for navigating your moves." << std::endl;
		std::cout << "'space' is used to proceed in text prompts or to select a move." << std::endl;
		std::cout << "The 'f' key can be used in a battle to force end it." << std::endl << std::endl;
		std::cout << "-- Gameplay --" << std::endl;
		std::cout << "The goal of the game is for you to beat as many pokemons possible." << std::endl;
		std::cout << "the score gets printed in the console after every battle." << std::endl;
		std::cout << "You have a chance to encounter pokemons by entering grass tiles." << std::endl;
		std::cout << "Pokemons can also move in grass tiles so they might jump you." << std::endl;
		std::cout << "Which pokemon you encounter is random could be a easy one or a legendary." << std::endl;
		std::cout << "The chance of encountering a legendary pokemon gets larger with every beaten pokemon." << std::endl;
		std::cout << "The last thing you need to know is that you can get your pokemon healed with the items (pokeballs) on the map which have a chance to spawn." << std::endl << std::endl;
	}
}
#pragma endregion ownDefinitions