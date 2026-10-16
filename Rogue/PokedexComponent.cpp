#include "PokedexComponent.h"

// Minigin
#include "ResourceManager.h"
#include "GameObject.h"

// Rogue
#include "Helpers.h"

// Standart Library
#include <algorithm>

using namespace Minigin;

PokedexComponent::PokedexComponent(GameObject* owner) :
	Component{ owner },
	m_Elements{},
	m_Moves{},
	m_Pokemon{},
	m_TypeMatchups{}
{
	ReadBinaryFile(m_Elements, ResourceManager::Instance()->GetFileRootPath() / "Elements.bin");
	ReadBinaryFile(m_Moves, ResourceManager::Instance()->GetFileRootPath() / "Moves.bin");
	ReadBinaryFile(m_Pokemon, ResourceManager::Instance()->GetFileRootPath() / "Pokemon.bin");
	MakeTypeMatchups();
}

const PODElement& PokedexComponent::GetElement(size_t index)
{
	if (index <= 0 or index > m_Elements.size())
	{
		throw std::runtime_error{ "GetElement(...) Invalid index given" };
	}

	return m_Elements.at(index - 1);
}

const PODMove& PokedexComponent::GetMove(size_t index)
{
	if (index <= 0 or index > m_Moves.size())
	{
		throw std::runtime_error{ "GetMove(...) Invalid index given" };
	}

	return m_Moves.at(index - 1);
}

const PODPokemon& PokedexComponent::GetPokemon(size_t index)
{
	if (index <= 0 or index > m_Pokemon.size())
	{
		throw std::runtime_error{ "GetPokemon(...) Invalid index given" };
	}

	return m_Pokemon.at(index - 1);
}

float PokedexComponent::GetTypeEffectiveness(size_t move, size_t defender)
{
	float effectiveness{ 1.0f };

	for (uint8_t elementIndex : m_Pokemon.at(defender - 1).Types)
	{
		// Element index 0 == no element
		if (elementIndex == 0) continue;

		size_t matchupIndex{ static_cast<size_t>(((m_Moves.at(move - 1).ElementType - 1) * POKEDEX_ELEMENT_COUNT) + (elementIndex - 1)) };
		effectiveness *= m_TypeMatchups.at(matchupIndex);
	}

	return effectiveness;
}

void PokedexComponent::MakeTypeMatchups()
{
	for (size_t attackingElementIndex{ 0 }; attackingElementIndex < POKEDEX_ELEMENT_COUNT; ++attackingElementIndex)
	{
		const PODElement& attackingElement{ m_Elements.at(attackingElementIndex) };

		for (size_t defendingElementIndex{ 0 }; defendingElementIndex < POKEDEX_ELEMENT_COUNT; ++defendingElementIndex)
		{
			const PODElement& defendingElement{ m_Elements.at(defendingElementIndex) };

			size_t typeMatchupIndex{ static_cast<size_t>(((attackingElementIndex) * POKEDEX_ELEMENT_COUNT) + (defendingElementIndex)) };
			
			// Ineffective
			if (attackingElement.IneffectiveAgainst == defendingElement.ID)
			{
				m_TypeMatchups.at(typeMatchupIndex) = 0.0f;
			}
			// Super Effective
			else if (std::ranges::any_of(attackingElement.SuperEffectiveAgainst, [&defendingElement](uint8_t index) -> bool { return index == defendingElement.ID; }))
			{
				m_TypeMatchups.at(typeMatchupIndex) = 2.0f;
			}
			// Not Very Effective
			else if (std::ranges::any_of(attackingElement.NotVeryEffectiveAgainst, [&defendingElement](uint8_t index) -> bool { return index == defendingElement.ID; }))
			{
				m_TypeMatchups.at(typeMatchupIndex) = 0.5f;
			}
			//Normal Effective
			else
			{
				m_TypeMatchups.at(typeMatchupIndex) = 1.0f;
			}
		}
	}
}