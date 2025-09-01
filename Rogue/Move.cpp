#include "Move.h"

// Core
#include "Text.h"
#include "ResourceManager.h"

using namespace Minigin;

Move::Move(const PODMove& move, Font* font) :
	NameText{ new Text{ GetDisplayName(move.Name), font, Color::Black } },
	Power{ move.Power }
{

}

std::string Move::GetDisplayName(const std::string& name)
{
	std::string displayName{ name };

	const size_t spacePosition{ displayName.find('-') };

	// Single word
	if (spacePosition == std::string::npos)
	{
		displayName.at(0) = static_cast<char>(std::toupper(displayName.at(0)));
	}
	// Two words
	else
	{
		// Fix spacing
		displayName.replace(spacePosition, 1, " ");

		// Fix casing
		displayName.at(0) = static_cast<char>(std::toupper(displayName.at(0)));
		displayName.at(spacePosition + 1) = static_cast<char>(std::toupper(displayName.at(spacePosition + 1)));
	}

	return displayName;
}