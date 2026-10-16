#include <filesystem>
#include <istream>
#include <fstream>
#include <cassert>
#include <json.hpp>
#include <array>

#include "Pokedex.hpp"

void ReadElements(std::array<PODElement, POKEDEX_ELEMENT_COUNT>& elements, const std::filesystem::path& path);
void WriteElements(const std::array<PODElement, POKEDEX_ELEMENT_COUNT>& elements, const std::filesystem::path& path);
void ReadMoves(std::array<PODMove, POKEDEX_MOVE_COUNT>& moves, const std::filesystem::path& path);
void WriteMoves(const std::array<PODMove, POKEDEX_MOVE_COUNT>& moves, const std::filesystem::path& path);
void ReadPokemon(std::array<PODPokemon, POKEDEX_POKEMON_COUNT>& pokemon, const std::filesystem::path& path);
void WritePokemon(const std::array<PODPokemon, POKEDEX_POKEMON_COUNT>& pokemon, const std::filesystem::path& path);

int main(int, char* [])
{
	// Read Write elements
	const std::filesystem::path elementsInputFilePath{ "Elements.json" };
	assert(std::filesystem::exists(elementsInputFilePath));
	const std::filesystem::path elementsOutputFilePath{ "Elements.bin" };
	std::array<PODElement, POKEDEX_ELEMENT_COUNT>* elements{ new std::array<PODElement, POKEDEX_ELEMENT_COUNT>{} };
	ReadElements(*elements, elementsInputFilePath);
	WriteElements(*elements, elementsOutputFilePath);

	// Read Write moves
	const std::filesystem::path movesInputFilePath{ "Moves.json" };
	assert(std::filesystem::exists(movesInputFilePath));
	const std::filesystem::path movesOutputFilePath{ "Moves.bin" };
	std::array<PODMove, POKEDEX_MOVE_COUNT>* moves{ new std::array<PODMove, POKEDEX_MOVE_COUNT>{} };
	ReadMoves(*moves, movesInputFilePath);
	WriteMoves(*moves, movesOutputFilePath);

	// Read Write pokemon
	const std::filesystem::path pokemonInputFilePath{ "Pokemon.json" };
	assert(std::filesystem::exists(pokemonInputFilePath));
	const std::filesystem::path pokemonOutputFilePath{ "Pokemon.bin" };
	std::array<PODPokemon, POKEDEX_POKEMON_COUNT>* pokemon{ new std::array<PODPokemon, POKEDEX_POKEMON_COUNT>{} };
	ReadPokemon(*pokemon, pokemonInputFilePath);
	WritePokemon(*pokemon, pokemonOutputFilePath);

	delete elements;
	delete moves;
	delete pokemon;

	return 0;
}

void ReadElements(std::array<PODElement, POKEDEX_ELEMENT_COUNT>& elements, const std::filesystem::path& path)
{
	std::ifstream inputFile{ path };
	if (!inputFile.is_open())
	{
		assert(false);
	}

	nlohmann::json jsonData{};
	inputFile >> jsonData;

	size_t elementIndex{ 0 };

	for (const auto& elementObject : jsonData)
	{
		PODElement* currentElement{ &elements.at(elementIndex) };

		// Name
		const std::string name{ elementObject.at("Name").get<std::string>() };
		strncpy_s(currentElement->Name, name.c_str(), _TRUNCATE);

		// ID
		currentElement->ID = elementObject.at("ID").get<uint8_t>();
		
		// SuperEffectiveAgainst
		const auto& superEffectiveAgainstData{ elementObject.at("SuperEffectiveAgainst") };
		for (size_t i = 0; i < 5; ++i)
		{
			currentElement->SuperEffectiveAgainst[i] = superEffectiveAgainstData[i].get<uint8_t>();
		}

		// NotVeryEffectiveAgainst
		const auto& notVeryEffectiveAgainstData{ elementObject.at("NotVeryEffectiveAgainst") };
		for (size_t i = 0; i < 7; ++i)
		{
			currentElement->NotVeryEffectiveAgainst[i] = notVeryEffectiveAgainstData[i].get<uint8_t>();
		}

		// IneffectiveAgainst
		const auto& ineffectiveAgainstData{ elementObject.at("IneffectiveAgainst") };
		for (size_t i = 0; i < 1; ++i)
		{
			currentElement->IneffectiveAgainst = ineffectiveAgainstData[i].get<uint8_t>();
		}

		++elementIndex;

		std::cout << *currentElement << std::endl;
	}
}

void WriteElements(const std::array<PODElement, POKEDEX_ELEMENT_COUNT>& elements, const std::filesystem::path& path)
{
	std::ofstream outputFile{ path, std::ios_base::binary };
	if (!outputFile.is_open())
	{
		assert(false);
	}

	outputFile.write(reinterpret_cast<const char*>(elements.data()), static_cast<std::streamsize>(elements.size() * sizeof(PODElement)));
}

void ReadMoves(std::array<PODMove, POKEDEX_MOVE_COUNT>& moves, const std::filesystem::path& path)
{
	std::ifstream inputFile{ path };
	if (!inputFile.is_open())
	{
		assert(false);
	}

	nlohmann::json jsonData{};
	inputFile >> jsonData;

	size_t moveIndex{ 0 };

	for (const auto& moveObject : jsonData)
	{
		PODMove* currentMove{ &moves.at(moveIndex) };

		// Name
		const std::string name{ moveObject.at("Name").get<std::string>() };
		strncpy_s(currentMove->Name, name.c_str(), _TRUNCATE);

		// ID
		currentMove->ID = moveObject.at("ID").get<uint8_t>();

		// MoveType
		currentMove->MoveType = moveObject.at("MoveType").get<uint8_t>();

		// ElementType
		currentMove->ElementType = moveObject.at("ElementType").get<uint8_t>();

		// Power
		currentMove->Power = moveObject.at("Power").get<uint8_t>();

		// Accuracy
		currentMove->Accuracy = moveObject.at("Accuracy").get<uint8_t>();

		++moveIndex;

		std::cout << *currentMove << std::endl;
	}
}

void WriteMoves(const std::array<PODMove, POKEDEX_MOVE_COUNT>& moves, const std::filesystem::path& path)
{
	std::ofstream outputFile{ path, std::ios_base::binary };
	if (!outputFile.is_open())
	{
		assert(false);
	}

	outputFile.write(reinterpret_cast<const char*>(moves.data()), static_cast<std::streamsize>(moves.size() * sizeof(PODMove)));
}

void ReadPokemon(std::array<PODPokemon, POKEDEX_POKEMON_COUNT>& pokemon, const std::filesystem::path& path)
{
	
	std::ifstream inputFile{ path };
	if (!inputFile.is_open())
	{
		assert(false);
	}

	nlohmann::json jsonData{};
	inputFile >> jsonData;

	size_t pokemonIndex{ 0 };

	for (const auto& pokemonObject : jsonData)
	{
		PODPokemon* currentPokemon{ &pokemon.at(pokemonIndex) };

		// Name
		const std::string name{ pokemonObject.at("Name").get<std::string>() };
		strncpy_s(currentPokemon->Name, name.c_str(), _TRUNCATE);

		// ID
		currentPokemon->ID = pokemonObject.at("ID").get<uint8_t>();

		// PhysicalAttack
		currentPokemon->PhysicalAttack = pokemonObject.at("PhysicalAttack").get<uint8_t>();

		// SpecialAttack
		currentPokemon->SpecicalAttack = pokemonObject.at("SpecialAttack").get<uint8_t>();

		// PhysicalDefense
		currentPokemon->PhysicalDefense = pokemonObject.at("PhysicalDefense").get<uint8_t>();

		// SpecialDefense
		currentPokemon->SpecialDefense = pokemonObject.at("SpecialDefense").get<uint8_t>();

		// Speed
		currentPokemon->Speed = pokemonObject.at("Speed").get<uint8_t>();

		// Health
		currentPokemon->Health = pokemonObject.at("Health").get<uint8_t>();

		// Moves
		const auto& movesData{ pokemonObject.at("MoveIDs") };
		for (size_t i = 0; i < 4; ++i)
		{
			currentPokemon->Moves[i] = movesData[i].get<uint8_t>();
		}

		// Types
		const auto& typesData{ pokemonObject.at("ElementTypes") };
		for (size_t i = 0; i < 2; ++i)
		{
			currentPokemon->Types[i] = typesData[i].get<uint8_t>();
		}

		++pokemonIndex;

		std::cout << *currentPokemon << std::endl;
	}
}

void WritePokemon(const std::array<PODPokemon, POKEDEX_POKEMON_COUNT>& pokemon, const std::filesystem::path& path)
{
	std::ofstream outputFile{ path, std::ios_base::binary };
	if (!outputFile.is_open())
	{
		assert(false);
	}

	outputFile.write(reinterpret_cast<const char*>(pokemon.data()), static_cast<std::streamsize>(pokemon.size() * sizeof(PODPokemon)));
}