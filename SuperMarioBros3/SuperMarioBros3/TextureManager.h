#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Type.h"
#include <unordered_map>
#include "Ultis.h"

class CTextureManager;
typedef CTextureManager* LPTextureManager;
class CTextureManager
{
private:
	static LPTextureManager instance;
	std::unordered_map<std::string, Texture> textures;

public:
	static LPTextureManager GetInstance();
	void Init();
	void Clear();
	void ClearTextureById(std::string texname);
	void LoadTexture(std::string texName, std::string texPath);
	Texture GetTexture(std::string id);
	void Add(std::string id, std::string filePath, Color transparentColor);
	~CTextureManager();
};

#endif