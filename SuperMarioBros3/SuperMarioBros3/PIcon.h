#pragma once
#include "Sprite.h"
class CPIcon
{
private:
	Point pos;
	LPSprite whitePowerIconSprite;
	LPSprite blackPowerIconSprite;
	LPSprite currentSprite;
	bool isCharged;
public:
	CPIcon();
	void LoadSprite();
	void Render();
	void SetPosition(Point pos);
	void SetCharged(bool isCharged);
	bool IsCharged();
};

