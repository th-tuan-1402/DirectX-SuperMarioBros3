#pragma once
#include "Sprite.h"
class CArrowItemIcon
{
private:
	Point pos;
	Point size;

	LPSprite blackArrowSprite;
	LPSprite whiteArrowSprite;
	LPSprite currentSprite;
	bool isCharged;
public:
	CArrowItemIcon();
	void LoadSprite();
	void SetCharged(bool isCharged);
	bool IsCharged();
	void Update();
	void Render();

	Point GetPosition();
	void SetPosition(Point pos);

	Point GetSize();
	void SetWidth(float w);
	void SetHeight(float h);
};

