#include "pch.h"
#include "MyUtils.h"
#include "utils.h"

extern float g_WindowWidth;

extern float g_WindowHeight;

namespace MyUtils
{
    int GetRandomNumber(const int begin, const int end)
    {
        const int randomNumber{ (rand() % (end + 1 - begin)) + begin };
        return randomNumber;
    }

    float GetRandomNumber(const float begin, const float end)
    {
        float random = ((float)rand()) / (float)RAND_MAX;
        float diff = end - begin;
        float r = random * diff;
        return begin + r;
    }

    Color4f GetRandomColor(const float alpha)
    {
        const float r{ GetRandomNumber(0.0f, 1.0f)};

        const float g{ GetRandomNumber(0.0f, 1.0f) };

        const float b{ GetRandomNumber(0.0f, 1.0f) };

        return Color4f{ r, g, b, alpha };
    }
     
    void SetRandomPosition(Circlef& circle, const float radius, const float margin)
    {
        const float posX{ rand() % int(g_WindowWidth - 2 * radius - 2 * margin) + radius + margin };
        const float posY{ rand() % int(g_WindowHeight - 2 * radius - 2 * margin) + radius + margin };
        const Point2f center{ posX, posY };
        circle.radius = radius;
        circle.center = center;
    }

    void SetRandomPosition(Rectf& rectangle, const float width, const float height, const float margin)
    {
        const float posX{ rand() % int(g_WindowWidth - 2 * margin - width) + margin };
        const float posY{ rand() % int(g_WindowHeight - 2 * margin - height) + margin };

        rectangle.left = posX;
        rectangle.bottom = posY;
        rectangle.width = width;
        rectangle.height = height;
    }

    int GetIndex2DArray(const int rowIndex, const int collIndex, const int nrCols)
    {
        return ((rowIndex * nrCols) + collIndex);
    }

    Rectf GetRectf2DArray(const int rowIndex, const int collIndex, const Point2f& startPosition, const float width, const float heigth, const float spacing)
    {
        const Rectf rectangle{ startPosition.x + (collIndex * (spacing + width)), startPosition.y + (rowIndex * (spacing + heigth)), width, heigth };
        return rectangle;
    }

    void UpdateSprite(Sprite& sprite, const float elapsedSec)
    {
        sprite.accumulatedTime += elapsedSec;
        if (sprite.accumulatedTime > sprite.frameTime)
        {
            ++sprite.currentFrame;
            sprite.currentFrame %= sprite.frames;
            sprite.accumulatedTime -= sprite.frameTime;
        }
    }

    void DrawSprite(const Sprite& sprite, const Point2f& position, const float scale)
    {
        Rectf srcRect{};

        srcRect.width = sprite.texture.width / sprite.cols;
        srcRect.height = sprite.texture.height / sprite.rows;
        srcRect.left = (sprite.currentFrame % sprite.cols) * srcRect.width;
        srcRect.bottom = (float(sprite.currentFrame / sprite.cols) + 1.0f) * srcRect.height;

        Rectf destRect{};
        destRect.left = position.x;
        destRect.bottom = position.y;
        destRect.width = srcRect.width * scale;
        destRect.height = srcRect.height * scale;

        utils::DrawTexture(sprite.texture, destRect, srcRect);
    }

    bool GetChance(const float chance)
    {
        const float number = GetRandomNumber(0.0f, 100.0f);
        return (number <= chance);
    }
}