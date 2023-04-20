#include "ArrowItemIcon.h"
#include "SpriteManager.h"
#include "Game.h"
#include "Const.h"
CArrowItemIcon::CArrowItemIcon()
{
	LoadSprite();
	isCharged = false;
}

void CArrowItemIcon::LoadSprite()
{
	auto spriteManager = CSpriteManager::GetInstance();
	blackArrowSprite = spriteManager->Get("spr-arrow-icon-black");
	whiteArrowSprite = spriteManager->Get("spr-arrow-icon-white");
	size.x = blackArrowSprite->GetWidth();
	size.y = blackArrowSprite->GetHeight();
}

void CArrowItemIcon::SetCharged(bool isCharged)
{
	this->isCharged = isCharged;
}

bool CArrowItemIcon::IsCharged()
{
	return isCharged;
}

void CArrowItemIcon::Update()
{

}

void CArrowItemIcon::Render()
{
	if (isCharged == false)
	{
		// Black
		currentSprite = blackArrowSprite;
	}
	else 
	{
		// White
		currentSprite = whiteArrowSprite;
	}
	size.x = currentSprite->GetWidth();
	size.y = currentSprite->GetHeight();

	currentSprite->Draw(pos, Point(1.0f, 1.0f), 0.0f);
}

Point CArrowItemIcon::GetPosition()
{
	return pos;
}

void CArrowItemIcon::SetPosition(Point pos)
{
	this->pos = pos;
}

Point CArrowItemIcon::GetSize()
{
	return size;
}

void CArrowItemIcon::SetWidth(float w)
{
	size.x = w;
}

void CArrowItemIcon::SetHeight(float h)
{
	size.y = h;
}
