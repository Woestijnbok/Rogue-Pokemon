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

PokemonStats::PokemonStats(uint16_t health, uint16_t speed, uint16_t attack, uint16_t defense, uint16_t specialAttack, uint16_t specialDefense) :
	Health{ health },
	Attack{ attack },
	SpecialAttack{ specialAttack },
	Defense{ defense },
	SpecialDefense{ specialDefense },
	Speed{ speed }
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

PokemonStats PokemonStats::operator+(uint16_t levels) const
{
	PokemonStats newStats{ *this };

	newStats.Health += levels;
	newStats.Speed += levels;
	newStats.SpecialAttack += levels;
	newStats.Attack += levels;
	newStats.Defense += levels;
	newStats.SpecialDefense += levels;

	return newStats;
}