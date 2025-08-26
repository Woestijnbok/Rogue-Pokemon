#pragma once

#include <cstdint>
#include <memory>
#include <random>

#include "Component.h"
#include "Subject.h"

namespace Minigin
{
	class Texture;
}

class PokemonComponent;
class TrainerComponent;

class BattleManagerComponent final : public Minigin::Component
{
public:
	explicit BattleManagerComponent(Minigin::GameObject* owner);
	virtual ~BattleManagerComponent() = default;

	BattleManagerComponent(const BattleManagerComponent& other) = delete;
	BattleManagerComponent(BattleManagerComponent&& other) noexcept = delete;
	BattleManagerComponent& operator=(const BattleManagerComponent& other) = delete;
	BattleManagerComponent& operator=(BattleManagerComponent&& other) noexcept = delete;

	virtual void Render() const override;

	void MakeBattle(TrainerComponent* trainer);
	void EndBattle();
	Minigin::Subject<>& OnBattleStarted();
	Minigin::Subject<>& OnBattleFinished();

private:
	PokemonComponent* ReadPokemon(uint8_t pokedexIndex) const;
	void StartBattle(PokemonComponent* player, PokemonComponent* enemy);

	std::pair<PokemonComponent*, PokemonComponent*> m_CurrentBattle;
	Minigin::Subject<> m_OnBattleStarted;
	Minigin::Subject<> m_OnBattleFinished;
	std::unique_ptr<Minigin::Texture> m_BattleBackground;
	std::unique_ptr<Minigin::Texture> m_TrainerPokemonTexture;
	std::unique_ptr<Minigin::Texture> m_EnemyPokemonTexture;
	std::random_device m_RandomDevice;
	std::mt19937 m_RandomEngine;
	std::uniform_int_distribution<int> m_RandomDistribution;
};	