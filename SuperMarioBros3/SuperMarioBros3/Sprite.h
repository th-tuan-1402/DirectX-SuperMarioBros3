#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include "Type.h"
#include "D3DType.h"
#include <string>

class CSprite;
typedef CSprite* LPSprite;
class CSprite
{
	std::string id;				// Sprite ID in the sprite database
	RECT rect;
	int width;
	int height;
	Point pointCenter;
	Texture texture;
	D3DXCOLOR transcolor;
	
public:
	CSprite(std::string id, int xPivot, RECT rect = RECT(), Texture tex = NULL, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255)); // texture là bên Texture manager giữ
	void Draw(Point position, Point scale, float rotation, D3DXCOLOR transcolor = D3DXCOLOR(254, 255, 255, 255));

	RECT GetRect();
	void SetRect(RECT rect);
	int GetWidth();
	void SetWidth(int w);
	int GetHeight();
	void SetHeight(int h);

	Point GetPointCenter();
	void SetPointCenter(Point pointCenter);

	D3DXCOLOR GetTranscolor();
	void SetTranscolor(D3DXCOLOR transcolor);
	~CSprite();
};

#endif