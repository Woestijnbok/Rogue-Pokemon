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