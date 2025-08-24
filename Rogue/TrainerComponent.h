#pragma once

#include <memory>

#include "Component.h"

class TrainerComponent final : public Minigin::Component
{
public:
	explicit TrainerComponent(Minigin::GameObject* owner);
	virtual ~TrainerComponent() = default;

	TrainerComponent(const TrainerComponent& other) = delete;
	TrainerComponent(TrainerComponent&& other) noexcept = delete;
	TrainerComponent& operator=(const TrainerComponent& other) = delete;
	TrainerComponent& operator=(TrainerComponent&& other) noexcept = delete;

private:
	
};