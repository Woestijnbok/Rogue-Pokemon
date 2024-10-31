#pragma once
#include "structs.h"

namespace MyUtils
{
	int GetRandomNumber(const int begin, const int end);

	float GetRandomNumber(const float begin, const float end);

	Color4f GetRandomColor(const float alpha = 1.0f);

	void SetRandomPosition(Circlef& circle, const float radius, const float margin = 0.0f);

	void SetRandomPosition(Rectf& rectangle, const float width, const float height, const float margin = 0.0f);

	// void Swap(int* pArr, const int idx1, const int idx2);

	int GetIndex2DArray(const int rowIndex, const int collIndex, const int nrCols);

	Rectf GetRectf2DArray(const int rowIndex, const int collIndex, const Point2f& startPosition, const float width, const float heigth, const float spacing = 0.0f);

	void UpdateSprite(Sprite& sprite, const float elapsedSec);

	void DrawSprite(const Sprite& sprite, const Point2f& position, const float scale = 1.0f);

	bool GetChance(const float chance);
}