#include "PokedexComponent.h"

// Minigin
#include "ResourceManager.h"
#include "GameObject.h"

// Rogue
#include "Helpers.h"

using namespace Minigin;

PokedexComponent::PokedexComponent(GameObject* owner) :
	Component{ owner },
	m_Elements{},
	m_Moves{},
	m_Pokemon{}
{
	ReadBinaryFile(m_Elements, ResourceManager::Instance()->GetFileRootPath() / "Elements.bin");
	ReadBinaryFile(m_Moves, ResourceManager::Instance()->GetFileRootPath() / "Moves.bin");
	ReadBinaryFile(m_Pokemon, ResourceManager::Instance()->GetFileRootPath() / "Pokemon.bin");
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