#include <cmath>

#include "Collision.h"

bool Minigin::PointInsideRectangle(const glm::ivec2& point, const Rectangle& rectangle)	
{
	// Not too high
	if (point.y <= rectangle.TopRight.y)	
	{
		// Not too right
		if (point.x <= rectangle.TopRight.x)	
		{
			// Not too low
			if (point.y >= rectangle.BottomLeft.y)
			{
				// Not too left
				if (point.x >= rectangle.BottomLeft.x)
				{
					return true;
				}
			}
		}
	}

	return false;
}

std::optional<glm::ivec2> Minigin::LinesIntersect(const Line& a, const Line& b)	
{
	std::optional<glm::ivec2> intersection{};

	const glm::vec2 directionOne{ a.End - a.Start }; // Direction vector of the first line segment						
	const glm::vec2 directionTwo{ b.End - b.Start }; // Direction vector of the second line segment					

	const float determinant{ directionOne.x * directionTwo.y - directionOne.y * directionTwo.x }; // Determinant				
	const float cross{ (b.Start - a.Start).x * directionOne.y - (b.Start - a.Start).y * directionOne.x }; // Cross product of vectors								

	if (std::fabs(determinant) < std::numeric_limits<float>::epsilon())
	{
		return intersection; // Lines are parallel or collinear	
	}

	const float t{ cross / determinant };
	const float u{ ((b.Start - a.Start).x * directionTwo.y - (b.Start - a.Start).y * directionTwo.x) / determinant };

	if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
	{
		intersection = a.Start + glm::ivec2{ t * directionOne };
		return intersection;
	}
	else return intersection;
}

bool Minigin::RectanglesOverlap(const Rectangle& a, const Rectangle& b)
{
	bool overlap{ true };

	// If one rectangle is to the left of the other
	if (a.TopRight.x < b.BottomLeft.x || b.TopRight.x < a.BottomLeft.x)	
	{
		overlap = false;
	}
	// If one rectangle is above the other
	else if (a.TopRight.y < b.BottomLeft.y || b.TopRight.y < a.BottomLeft.y)	
	{
		overlap = false;
	}

	return overlap;
}
