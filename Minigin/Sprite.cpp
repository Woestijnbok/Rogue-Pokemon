#include "Sprite.h"
#include "Texture.h"

using namespace Minigin;

Sprite::Sprite(Texture* sheet, int frames, int rows, int collumns) :
	m_Sheet{ sheet },
	m_Frames{ frames },
	m_Rows{ rows },
	m_Collumns{ collumns }
{

}

Texture* Sprite::GetSheet() const
{
	return m_Sheet.get();	
}

int Sprite::GetFrames() const
{
	return m_Frames;
}

int Sprite::GetRows() const
{
	return m_Rows;
}

int Sprite::GetCollumns() const
{
	return m_Collumns;
}

glm::ivec2 Sprite::GetFrameSize() const
{
	const glm::ivec2 sheetSize{ m_Sheet->GetSize() };

	return glm::ivec2{ sheetSize.x / m_Collumns, sheetSize.y / m_Rows };
}
