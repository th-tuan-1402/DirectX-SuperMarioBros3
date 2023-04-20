#pragma once
#include "Sprite.h"
#include <map>
class CCardGift
{
private:
	std::map<std::string, LPSprite> cardSprites;
	Point pos;
	std::string state;
public:
	CCardGift();
	CCardGift(Point cardPos);
	void LoadSprite();
	void Update();
	void Render();
	void SetState(std::string state);
	void SetPosition(Point pos);
	std::string GetState();
};