#include <filesystem>
#include <istream>
#include <fstream>
#include <regex>
#include <cassert>
#include <json.hpp>
#include <array>

#include "Pokedex.hpp"

int ReadPokedex(std::array<PODPokemon, POKEDEX_COUNT>& pokedex, const std::filesystem::path& path);
int WritePokedex(const std::array<PODPokemon, POKEDEX_COUNT>& pokedex, const std::filesystem::path& path);
#ifdef _DEBUG
void TestReadPokedex(const std::filesystem::path& path, uint8_t startIndex, uint8_t amount);
#endif

int main(int, char* [])
{
	const std::filesystem::path inputFilePath{ "Pokedex.json" };
	assert(std::filesystem::exists(inputFilePath));

	const std::filesystem::path outputFilePath{ "Pokedex.bin" };

	std::array<PODPokemon, POKEDEX_COUNT> pokedex{};

	ReadPokedex(pokedex, inputFilePath);
	WritePokedex(pokedex, outputFilePath);
#ifdef _DEBUG
	TestReadPokedex(outputFilePath, 5, 5);
#endif

	return 0;
}

int ReadPokedex(std::array<PODPokemon, POKEDEX_COUNT>& pokedex, const std::filesystem::path& path)
{
	std::ifstream inputFile{ path };
	assert(inputFile.is_open());

	nlohmann::json jsonData{};
	inputFile >> jsonData;

	size_t pokedexIndex{ 0 };

	for (const auto& pokemonObject : jsonData)
	{
		PODPokemon* currentPokemon{ &pokedex.at(pokedexIndex) };

		currentPokemon->Index = pokemonObject.at("index").get<uint8_t>();
		const std::string name{ pokemonObject.at("name").get<std::string>() };
		strncpy_s(currentPokemon->Name, name.c_str(), _TRUNCATE);

		const auto& movesData{ pokemonObject.at("moves") };
		for (size_t i = 0; i < 4; ++i)
		{
			const std::string moveName{ movesData[i].at("name").get<std::string>() };
			strncpy_s(currentPokemon->Moves[i].Name, moveName.c_str(), _TRUNCATE);
			currentPokemon->Moves[i].Power = movesData[i].at("power").get<uint8_t>();
		}

		++pokedexIndex;

		std::cout << *currentPokemon << std::endl;
	}

	return 0;
}

int WritePokedex(const std::array<PODPokemon, POKEDEX_COUNT>& pokedex, const std::filesystem::path& path)
{
	std::ofstream outputFile{ path, std::ios_base::binary };
	assert(outputFile.is_open());

	for (const auto& pokemon : pokedex)
	{
		outputFile.write(reinterpret_cast<const char*>(&pokemon), sizeof(PODPokemon));
	}

#ifdef _DEBUG
	std::cout << std::endl << "Finished Writing" << path;
	std::cout << std::endl << std::format("Each pokemon entry is {} bytes", sizeof(PODPokemon));
	std::cout << std::endl << std::format("Each move entry is {} bytes", sizeof(PODMove));
#endif

	return 0;
}

#ifdef _DEBUG
void TestReadPokedex(const std::filesystem::path& path, uint8_t startIndex, uint8_t amount)
{
	std::ifstream inputFile{ path, std::ios::binary };
	if (!inputFile)
	{
		std::cerr << "Failed to open file: " << path << "\n";
		return;
	}

	std::cout << std::endl << "Test " << path << std::endl;

	inputFile.seekg(sizeof(PODPokemon) * startIndex);

	for (std::size_t i = 0; i < amount; ++i)
	{
		PODPokemon currentPokemon{};
		inputFile.read(reinterpret_cast<char*>(&currentPokemon), sizeof(PODPokemon));

		if (!inputFile) break; // End of file

		std::cout << currentPokemon << std::endl;
	}
}
#endif