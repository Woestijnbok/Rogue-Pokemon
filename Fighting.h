#pragma once

extern float g_WindowWidth;

extern float g_WindowHeight;

#pragma region Structs
struct Attack
{
	std::string nameStr;
	float attack;
	float specialAttack;
	float upSpeed;
	float upAttack;
	float upSpecialAttack;
	float upDefence;
	float upSpecialDefence;
	Texture name;
	Texture attackEffect;
	float elapsedSec;
	bool isDynamic;

	Attack();
	Attack(std::string name, float attack);
	Attack(std::string name, float attack, bool dynamic);
};

struct Pokemon
{
	const float maxHealth;
	float health;
	const float maxDefence;
	float defence;
	const float maxSpecialDefence;
	float specialDefence;
	const float maxSpeed;
	float speed;
	const float maxAttack;
	float attack;
	const float maxSpecialAttack;
	float specialAttack;
	const std::string nameStr;
	bool isAlive;
	Texture name;
	Texture image;
	Attack moves[4]{};
	Pokemon();
	Pokemon(std::string nameStr, float health, float def, float speed, float power);
};
#pragma endregion Structs

#pragma region Enumerations
enum class PokemonStatus
{
	nothing,
	sleep,
	burn,
	freeze,
	posion,
	paralysis
};

enum class MoveSelecter
{
	leftDown,
	rightDown,
	leftUp,
	rightUp
};

enum class FightSequence
{
	start,
	selectMove,
	yourMove,
	yourText,
	enemyChoose,
	enemyMove,
	enemyText,
	winner,
	end
};
#pragma endregion Enumerations

#pragma region Variables
extern Texture g_BattleBackround;

extern const float g_ScalePokemonImage;

extern const Rectf g_RectfAlliedPokemon;

extern const Rectf g_RectfEnemyPokemon;

extern const float g_ScaleBattleBox;

extern const Rectf g_RectfBattleBox;

extern Texture g_InfoBox;

extern Texture g_SelectMoveBox;

extern const int g_TextSize;

extern Texture g_Appeared;

extern Texture g_Used;

extern Texture g_Fainted;

extern Texture g_Arrow;

extern const float g_ScaleCloud;

extern const Rectf g_RectfYourCloud;

extern const Rectf g_RectfEnemyCloud;

extern Texture g_YourCloud;

extern Texture g_EnemyCloud;

extern const int g_MaxNrMoves;

extern MoveSelecter g_YourChoice;

extern MoveSelecter g_EnemyChoice;

extern FightSequence g_FightSequence;

extern Pokemon g_AlliedPokemon;

extern const int g_NrLegendaryPokemons;

extern Pokemon g_ArrLegendaryPokemons[3];

extern const int g_NrEnemyPokemons;

extern Pokemon g_ArrEnemyPokemons[5];

extern Pokemon* g_pEnemyPokemon;

extern const int g_NameSize;

extern float g_ChanceLegendaryEncounter;
#pragma endregion Variables

#pragma region Attacks
extern Attack g_Tackle;

extern Attack g_Bite;

extern Attack g_Flamethrower;

extern Attack g_Growl;

extern Attack g_Tail;

extern Attack g_Quick;

extern Attack g_Rock;

extern Attack g_Sand;

extern Attack g_Shadowball;

extern Attack g_Hex;

extern Attack g_PsychoBoost;

extern Attack g_HyperBeam;

extern Attack g_ZenHeadbutt;

extern Attack g_Psychic;

extern Attack g_MegaPunch;

extern Attack g_Ancientpower;

extern Attack g_AuraSphere;

extern Attack g_Discharge;

extern Attack g_DrillPeck;

extern Attack g_Thunder;

extern Attack g_Tornado;

extern Attack g_Slash;

extern Attack g_Surf;
#pragma endregion Attacks

#pragma region Functions
void SetAttacks();

void InitializeAttacks();

void DeleteAttacks();

void InitializePokemons();

void DeletePokemons();

void InitializeBattle();

void DeleteBattle();

void SelectMove(SDL_Keycode key);

void ProceedBattle(SDL_Keycode key);

void ForceEndBattle(SDL_Keycode key);

void RegisterMove(float time);

void NewEnemyMove();

void UpdateBattle(float time);

void NewEnemy();

void ResetEnemies();

void TestDrawBattlePokemon();

void DrawSelectMoves();

void DrawInfo();

void DrawBattleBox();

void DrawHealth();

void DrawBattle();

void HealParty();

void GetLegendaryPokemon();

void DrawFightAnimation(const Pokemon& pokemon, const MoveSelecter& attackIndex, const Point2f& start, const Point2f& end);
#pragma endregion Functions