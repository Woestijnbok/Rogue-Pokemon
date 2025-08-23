#pragma once

#include <memory>

#include "Component.h"
#include "MoveCommand.h"

namespace Minigin
{
	class Sprite;
}

class TrainerComponent final : public Minigin::Component
{
public:
	explicit TrainerComponent(Minigin::GameObject* owner);
	virtual ~TrainerComponent() = default;

	TrainerComponent(const TrainerComponent& other) = delete;
	TrainerComponent(TrainerComponent&& other) noexcept = delete;
	TrainerComponent& operator=(const TrainerComponent& other) = delete;
	TrainerComponent& operator=(TrainerComponent&& other) noexcept = delete;

	virtual void Render() const override;

	void Move(MoveCommand::Direction direction);

private:
	void SetDirection(MoveCommand::Direction direction);

	std::unique_ptr<Minigin::Sprite> m_TrainerDownSprite;
	std::unique_ptr<Minigin::Sprite> m_TrainerUpSprite;
	std::unique_ptr<Minigin::Sprite> m_TrainerRightSprite;
	MoveCommand::Direction m_Direction;
};