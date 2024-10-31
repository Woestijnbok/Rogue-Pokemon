#pragma once

#include "pch.h"
#include "Exploring.h"

using namespace utils;
using namespace MyUtils;

void LoadTexturesExploring()
{
	const Color4f black{0.0f, 0.0f, 0.0f, 1.0f};

	TextureFromFile("Resources/Exploring/Dirt.png", g_Dirt);
	TextureFromFile("Resources/Exploring/Grass.png", g_Grass);
	TextureFromFile("Resources/Exploring/Pokeball.png", g_Pokeball);

	TextureFromFile("Resources/Exploring/Right.png", g_Right.texture);
	g_Right.rows = 3;
	g_Right.cols = 1;
	g_Right.frames = 3;
	g_Right.frameTime = 1.0f / 10.0f;

	TextureFromFile("Resources/Exploring/Left.png", g_Left.texture);
	g_Left.rows = 3;
	g_Left.cols = 1;
	g_Left.frames = 3;
	g_Left.frameTime = 1.0f / 10.0f;

	TextureFromFile("Resources/Exploring/Up.png", g_Up.texture);
	g_Up.rows = 3;
	g_Up.cols = 1;
	g_Up.frames = 3;
	g_Up.frameTime = 1.0f / 10.0f;

	TextureFromFile("Resources/Exploring/Down.png", g_Down.texture);
	g_Down.rows = 3;
	g_Down.cols = 1;
	g_Down.frames = 3;
	g_Down.frameTime = 1.0f / 10.0f;

	TextureFromString("We've restored your pokémon to full health.", "Resources/FireRed Font.ttf", g_TextSize, black, g_Healed);
}

void DeleteTexturesExploring()
{
	DeleteTexture(g_Dirt);
	DeleteTexture(g_Grass);
	DeleteTexture(g_Pokeball);
	DeleteTexture(g_Right.texture);
	DeleteTexture(g_Left.texture);
	DeleteTexture(g_Up.texture);
	DeleteTexture(g_Down.texture);
	DeleteTexture(g_Healed);
}

void SetSeedMap()
{
	for (int i{}; i < (g_NrRows * g_NrColls); ++i)
	{
		if (GetChance(g_GrassChance))
		{
			if (GetChance(g_PokemonChance)) g_pArrMap[i] = Tile::pokemon;
			else g_pArrMap[i] = Tile::grass;
		}
		else
		{
			if (GetChance(g_ItemChance)) g_pArrMap[i] = Tile::item;
			else g_pArrMap[i] = Tile::dirt;
		}
	}
}

void SetPlayer()
{
	g_pArrMap[GetIndex2DArray(4, 4, g_NrColls)] = Tile::dirt;
	Rectf tile = GetRectf2DArray(4, 4, Point2f{ 0.0f, 0.0f }, g_GridSize, g_GridSize);
	Point2f middleTile{ tile.left + (tile.width / 2.0f),tile.bottom + (tile.height / 2.0f) };
	g_Player = Circlef{ middleTile, 30.0f };
}

void DrawMap()
{
	for (int i{}; i < g_NrRows; ++i)
	{
		for (int j{}; j < g_NrColls; ++j)
		{
			switch (g_pArrMap[GetIndex2DArray(i, j, g_NrColls)])
			{
			case Tile::grass:
				SetColor(0.0f, 1.0f, 0.0f, 0.2f);
				FillRect(GetRectf2DArray(i, j, Point2f{ 0.0f, 0.0f }, g_GridSize, g_GridSize));
				break;
			case Tile::pokemon:
				SetColor(1.0f, 0.0f, 0.0f, 0.2f);
				FillRect(GetRectf2DArray(i, j, Point2f{ 0.0f, 0.0f }, g_GridSize, g_GridSize));
				break;
			case Tile::dirt:
				SetColor(0.2f, 0.2f, 0.2f, 0.2f);
				FillRect(GetRectf2DArray(i, j, Point2f{ 0.0f, 0.0f }, g_GridSize, g_GridSize));
				break;
			case Tile::item:
				SetColor(0.0f, 0.7f, 1.0f, 0.2f);
				FillRect(GetRectf2DArray(i, j, Point2f{ 0.0f, 0.0f }, g_GridSize, g_GridSize));
				break;
			}
		}
	}
}

void DrawPlayer()
{
	SetColor(0.0f, 0.0f, 1.0f, 0.2f);
	FillEllipse(g_Player.center, g_Player.radius, g_Player.radius);
}

void MovePokemon(const int row, const int coll)
{
	const int move = GetRandomNumber(0, 3);

	switch (move)
	{
	case 3:
		if (coll < g_NrColls && g_pArrMap[GetIndex2DArray(row, coll + 1, g_NrColls)] == Tile::grass)
		{
			g_pArrMap[GetIndex2DArray(row, coll, g_NrColls)] = Tile::grass;
			g_pArrMap[GetIndex2DArray(row, coll + 1, g_NrColls)] = Tile::pokemon;
		}
		break;
	case 2:
		if (coll > 0 && g_pArrMap[GetIndex2DArray(row, coll - 1, g_NrColls)] == Tile::grass)
		{
			g_pArrMap[GetIndex2DArray(row, coll, g_NrColls)] = Tile::grass;
			g_pArrMap[GetIndex2DArray(row, coll - 1, g_NrColls)] = Tile::pokemon;
		}
		break;
	case 1:
		if (row < g_NrRows && g_pArrMap[GetIndex2DArray(row + 1, coll, g_NrColls)] == Tile::grass)
		{
			g_pArrMap[GetIndex2DArray(row, coll, g_NrColls)] = Tile::grass;
			g_pArrMap[GetIndex2DArray(row + 1, coll, g_NrColls)] = Tile::pokemon;
		}
		break;
	default:
		if (row > 0 && g_pArrMap[GetIndex2DArray(row - 1, coll, g_NrColls)] == Tile::grass)
		{
			g_pArrMap[GetIndex2DArray(row, coll, g_NrColls)] = Tile::grass;
			g_pArrMap[GetIndex2DArray(row - 1, coll, g_NrColls)] = Tile::pokemon;
		}
		break;
	}
}

void MovePokemons(float elapsedSec)
{
	g_PokemonIdleTime += elapsedSec;
	if (g_PokemonIdleTime > g_PokemonMaxIdleTime)
	{
		for (int row{}; row < g_NrRows; ++row)
		{
			for (int coll{}; coll < g_NrColls; ++coll)
			{
				if (g_pArrMap[GetIndex2DArray(row, coll, g_NrColls)] == Tile::pokemon)
				{
					MovePokemon(row, coll);
				}
			}
		}
		g_PokemonIdleTime = 0.0f;
	}
}

void UpdateTexturePlayer(float elapsedSec)
{
	switch (g_PlayerMovement)
	{
	case Movement::right:
		UpdateSprite(g_Right, elapsedSec);
		break;
	case Movement::left:
		UpdateSprite(g_Left, elapsedSec);
		break;
	case Movement::up:
		UpdateSprite(g_Up, elapsedSec);
		break;
	case Movement::down:
		UpdateSprite(g_Down, elapsedSec);
		break;
	}
}

bool Moved()
{
	for (int row{}; row < g_NrRows; ++row)
	{
		for (int coll{}; coll < g_NrColls; ++coll)
		{
			const Rectf tile = GetRectf2DArray(row, coll, Point2f{}, g_GridSize, g_GridSize);
			if ((g_Player.center.x == (tile.left + (tile.width / 2))) && (g_Player.center.y == (tile.bottom + (tile.height / 2))))
			{
				return true;
			}
		}
	}
	return false;
}

void UpdatePlayer(float elapsedSec)
{
	if (g_PlayerMovement == Movement::right)
	{
		const Rectf rightMostTile{ GetRectf2DArray(0, g_NrColls - 1, Point2f{}, g_GridSize, g_GridSize) };
		const bool canGoRight{ g_Player.center.x < (rightMostTile.left + (rightMostTile.width / 2.0f)) };

		if (canGoRight)
		{
			g_MovementTimer += elapsedSec;

			if (g_MovementTimer > 0.1f)
			{
				g_Player.center.x += (g_GridSize / 5.0f);
				g_MovementTimer -= 0.1f;

				if (Moved())
				{
					g_PlayerMovement = Movement::still;
					g_MovementTimer = 0.0f;
					g_PlayerFacing = Facing::right;
					g_Right.currentFrame = 0;
					g_Right.accumulatedTime = 0.0f;
				}
			}
		}
		else g_PlayerMovement = Movement::still;
	}
	else if (g_PlayerMovement == Movement::left)
	{
		const Rectf leftMostTile{ GetRectf2DArray(0, 0, Point2f{}, g_GridSize, g_GridSize) };
		const bool canGoLeft{ g_Player.center.x > (leftMostTile.left + (leftMostTile.width / 2.0f)) };

		if (canGoLeft)
		{
			g_MovementTimer += elapsedSec;

			if (g_MovementTimer > 0.1f)
			{
				g_Player.center.x -= (g_GridSize / 5.0f);
				g_MovementTimer -= 0.1f;

				if (Moved())
				{
					g_PlayerMovement = Movement::still;
					g_MovementTimer = 0.0f;
					g_PlayerFacing = Facing::left;
					g_Left.currentFrame = 0;
					g_Left.accumulatedTime = 0.0f;
				}
			}
		}
		else g_PlayerMovement = Movement::still;
	}
	else if (g_PlayerMovement == Movement::up)
	{
		const Rectf upMostTile{ GetRectf2DArray(g_NrRows - 1, 0, Point2f{}, g_GridSize, g_GridSize) };
		const bool canGoUp{ g_Player.center.y < (upMostTile.bottom + (upMostTile.height / 2.0f)) };

		if (canGoUp)
		{
			g_MovementTimer += elapsedSec;

			if (g_MovementTimer > 0.1f)
			{
				g_Player.center.y += (g_GridSize / 5.0f);
				g_MovementTimer -= 0.1f;

				if (Moved())
				{
					g_PlayerMovement = Movement::still;
					g_MovementTimer = 0.0f;
					g_PlayerFacing = Facing::up;
					g_Up.currentFrame = 0;
					g_Up.accumulatedTime = 0.0f;
				}
			}
		}
		else g_PlayerMovement = Movement::still;
	}
	else if (g_PlayerMovement == Movement::down)
	{
		const Rectf downMostTile{ GetRectf2DArray(0, 0, Point2f{}, g_GridSize, g_GridSize) };;
		const bool canGoDown{ g_Player.center.y > (downMostTile.bottom + (downMostTile.height / 2.0f)) };

		if (canGoDown)
		{
			g_MovementTimer += elapsedSec;

			if (g_MovementTimer > 0.1f)
			{
				g_Player.center.y -= (g_GridSize / 5.0f);
				g_MovementTimer -= 0.1f;

				if (Moved())
				{
					g_PlayerMovement = Movement::still;
					g_MovementTimer = 0.0f;
					g_PlayerFacing = Facing::down;
					g_Down.currentFrame = 0;
					g_Down.accumulatedTime = 0.0f;
				}
			}
		}
		else g_PlayerMovement = Movement::still;
	}
}

void TexturizeMap()
{
	for (int row{}; row < g_NrRows; ++row)
	{
		for (int coll{}; coll < g_NrColls; ++coll)
		{
			Rectf tile{ GetRectf2DArray(row, coll, Point2f{}, g_GridSize, g_GridSize) };

			switch (g_pArrMap[GetIndex2DArray(row, coll, g_NrColls)])
			{
				case Tile::dirt:
					DrawTexture(g_Dirt, tile);
					break;
				case Tile::grass:
				case Tile::pokemon:
					DrawTexture(g_Grass, tile);
					break;
				case Tile::item:
					DrawTexture(g_Pokeball, tile);
					break;
			}
		}
	}
}

void TexturizePlayer()
{
	const Point2f position{ g_Player.center.x - 25.0f, g_Player.center.y - 27.0f };
	const float scale{ 3.5 };

	switch (g_PlayerMovement)
	{
	case Movement::still:
		switch (g_PlayerFacing)
		{
		case Facing::right:
			DrawSprite(g_Right, position, scale);
			break;
		case Facing::left:
			DrawSprite(g_Left, position, scale);
			break;
		case Facing::up:
			DrawSprite(g_Up, position, scale);
			break;
		case Facing::down:
			DrawSprite(g_Down, position, scale);
			break;
		}
		break;
	case Movement::right:
		DrawSprite(g_Right, position, scale);
		break;
	case Movement::left:
		DrawSprite(g_Left, position, scale);
		break;
	case Movement::up:
		DrawSprite(g_Up, position, scale);
		break;
	case Movement::down:
		DrawSprite(g_Down, position, scale);
		break;
	}
}

void MovePlayer(SDL_Keycode key)
{
	switch(key)
	{
		case SDLK_RIGHT:
			if (g_PlayerMovement == Movement::still)
			{
				g_PlayerMovement = Movement::right;
			}
			break;
		case SDLK_LEFT:
			if (g_PlayerMovement == Movement::still)
			{
				g_PlayerMovement = Movement::left;
			}
			break;
		case SDLK_UP:
			if (g_PlayerMovement == Movement::still)
			{
				g_PlayerMovement = Movement::up;
			}
			break;
		case SDLK_DOWN:
			if (g_PlayerMovement == Movement::still)
			{
				g_PlayerMovement = Movement::down;
			}
			break;
	}
}

int GetIndexPlayerTile()
{
	for (int row{}; row < g_NrRows; ++row)
	{
		for (int coll{}; coll < g_NrColls; ++coll)
		{
			const Rectf tile = GetRectf2DArray(row, coll, Point2f{}, g_GridSize, g_GridSize);
			if ((g_Player.center.x == (tile.left + (tile.width / 2))) && (g_Player.center.y == (tile.bottom + (tile.height / 2))))
			{
				return GetIndex2DArray(row, coll, g_NrColls);
			}
		}
	}

	return 100;
}//--------------------------------------

void DisplayPickup()
{
	Rectf position{ (g_WindowWidth / 2.0f) - (g_RectfBattleBox.width / 2.0f), 10.0f, g_RectfBattleBox.width, g_RectfBattleBox.height };
	DrawTexture(g_InfoBox, position);
	DrawTexture(g_Healed, Point2f{ position.left + 40.0f, position.bottom + (position.height / 2.0f) - 5.0f });
}