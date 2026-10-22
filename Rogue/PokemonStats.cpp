#include "PokemonStats.h"

PokemonStats::PokemonStats(const PODPokemon& pokemon) :
	Health{ pokemon.Health },
	Attack{ pokemon.PhysicalAttack },
	SpecialAttack{ pokemon.SpecicalAttack },
	Defense{ pokemon.PhysicalDefense },
	SpecialDefense{ pokemon.SpecialDefense },
	Speed{ pokemon.Speed }
{

}

PokemonStats& PokemonStats::operator++()
{
	++Health;
	++Speed;
	++SpecialAttack;
	++Attack;
	++Defense;
	++SpecialDefense;

	return *this;
}

void PokemonStats::operator+=(int levelDifference)
{
	Health = std::max(0, Health + levelDifference);
	Speed = std::max(0, Speed + levelDifference);
	Attack = std::max(0, Attack + levelDifference);
	Defense = std::max(0, Defense + levelDifference);
	SpecialAttack = std::max(0, SpecialAttack + levelDifference);
	SpecialDefense = std::max(0, SpecialDefense + levelDifference);
}