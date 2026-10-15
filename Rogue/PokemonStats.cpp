#include "PokemonStats.h"

PokemonStats::PokemonStats(const PODPokemon& pokemon) :
	Level{ 1 },
	MaxHealth{ pokemon.Health },
	CurrentHealth{ MaxHealth },
	Attack{ pokemon.PhysicalAttack },
	SpecialAttack{ pokemon.SpecicalAttack },
	Defense{ pokemon.PhysicalDefense },
	SpecialDefense{ pokemon.SpecialDefense },
	Speed{ pokemon.Speed }
{

}