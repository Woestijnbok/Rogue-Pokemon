#pragma once

#include "structs.h"

#pragma region Enumerations
enum class Tile
{
	dirt,
	grass,
	pokemon,
	item
};

enum class Movement
{
	still,
	right,
	left,
	up,
	down
};

enum class Facing
{
	right,
	left,
	up,
	down
};
#pragma endregion Enumerations

#pragma region Variables
extern const int g_TextSize;

extern Texture g_Healed;

extern const Rectf g_RectfBattleBox;

extern float g_WindowWidth;

extern Texture g_InfoBox;

extern Texture g_Healed;

extern Texture g_Grass;

extern Texture g_Dirt;

extern Sprite g_Right;

extern Sprite g_Left;

extern Sprite g_Up;

extern Sprite g_Down;

extern Facing g_PlayerFacing;

extern Movement g_PlayerMovement;

extern const float g_GridSize;

extern const int g_NrRows;

extern const int g_NrColls;

extern Tile* g_pArrMap;

extern const float g_GrassChance;

extern const float g_PokemonChance;

extern Circlef g_Player;

extern float g_MovementTimer;

extern int g_NrDefeatedenemies;

extern float g_PokemonIdleTime;

extern float g_PokemonMaxIdleTime;

extern const float g_ItemChance;

extern Texture g_Pokeball;
#pragma endregion Variables

#pragma region Functions
void LoadTexturesExploring();

void DeleteTexturesExploring();

void SetSeedMap();

void SetPlayer();

void DrawMap();

void DrawPlayer();

void MovePokemon(const int row, const int coll);

void MovePokemons(float elapsedSec);

void UpdateTexturePlayer(float elapsedSec);

bool Moved();

void UpdatePlayer(float elapsedSec);

void TexturizeMap();

void TexturizePlayer();

void MovePlayer(SDL_Keycode key);

int GetIndexPlayerTile();

void DisplayPickup();
#pragma endregion Functions