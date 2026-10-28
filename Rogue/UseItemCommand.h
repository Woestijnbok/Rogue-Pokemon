#pragma once

#include "Command.h"
#include "TrainerComponent.h"
#include "Enums.h"

class UseItemCommand : public Minigin::ComponentCommand<TrainerComponent>
{
public:
	UseItemCommand(TrainerComponent* trainerComponent, ItemType item);
	virtual ~UseItemCommand() = default;

	UseItemCommand(const UseItemCommand&) = delete;
	UseItemCommand(UseItemCommand&&) = delete;
	UseItemCommand& operator= (const UseItemCommand&) = delete;
	UseItemCommand& operator= (const UseItemCommand&&) = delete;

	virtual void Execute() override;

private:
	ItemType m_ItemType;
};