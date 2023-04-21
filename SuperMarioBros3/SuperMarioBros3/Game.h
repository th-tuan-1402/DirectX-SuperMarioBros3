#pragma once

#ifndef GAME_H
#define GAME_H

#include "D3DHelper.h"
#include "Type.h"
#include <dinput.h>
#include <unordered_map>

#include "KeyEventHandler.h"
#include "GameConst.h"

#include "KeyboardManager.h"
class CGame
{
private:
	int screenWidth;
	int screenHeight;

	int fps;

	static CGame* instance;
	static float timeScale;
	static DWORD deltaTime;

	HWND hWnd; // Window Handle

	// Sau khi đọc file root.xml, ta sẽ lưu các thông tin file đó dưới dạng 1 map chứa 1 map
	// Mỗi category (Textures, Sprites, Animations, Scenes, Config) sẽ có các bucket
	// các bucket sẽ giữ id và source (filePath)
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> gameSource;

	// DirectX Helper
	D3DHelper* d3dHelper;

public:
	static CGame* GetInstance();
	~CGame();
	void Draw(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255,0,255));
	void Draw(Point position, Texture texture, RECT rect, int alpha = 253);

	void DrawFlipX(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
	void DrawFlipY(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
	
	void Init(HWND hWnd, int scrWidth, int scrHeight, int fps);
	void Request();
	void Run(); // while-loop game
	void End();
	void Clean();
	void Render();
	void Update();

	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }

	Texture LoadTexture(std::string filePath, D3DCOLOR transparentColor) { return this->d3dHelper->LoadTexture(filePath, transparentColor); }
	
	static float GetTimeScale() { return timeScale; }
	static void SetTimeScale(float time) { timeScale = time; }

	float GetFixedDeltaTime() { return 20; }
	DWORD GetDeltaTime() { return deltaTime;  }

	bool ImportGameSource();
	String GetFilePathByCategory(String category, String id);
};

#endif