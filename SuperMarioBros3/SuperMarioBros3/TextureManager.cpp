
#include <string>

#include "TextureManager.h"
#include "Game.h"
#include "GraphicConst.h"
#include "Const.h"

using namespace std;

LPTextureManager CTextureManager::instance = NULL;


LPTextureManager CTextureManager::GetInstance()
{
	if (instance == NULL) instance = new CTextureManager();
	return instance;
}

void CTextureManager::Init()
{

}

void CTextureManager::Clear()
{
	for (auto t : textures)
	{
		Texture tex = t.second;
		// TODO: clear when refactor
		// if (tex != NULL) tex->Release();
	}
	textures.clear();
}

void CTextureManager::ClearTextureById(std::string texname)
{
	auto tex = GetTexture(texname);
	if (tex != nullptr)
	{
		textures.erase(texname);
		// TODO: clear when refactor
		//tex->Release();
		//tex = NULL;
	}
}

void CTextureManager::LoadTexture(string  texName, string texPath)
{
	Add(texName, texPath, D3DCOLOR_XRGB(255, 255, 255));
}

Texture CTextureManager::GetTexture(std::string id)
{
	if (textures.find(id) == textures.end())
		return nullptr;
	else 
		return textures.at(id);
}

void CTextureManager::Add(string id, std::string filePath, Color transparentColor)
{
	Texture texture = CGame::GetInstance()->LoadTexture(filePath);

	if (texture == NULL)
	{
		OutputDebugStringW(ToLPCWSTR("[INFO] Texture loaded fail:  " + filePath + "\n"));
		return;
	}

	textures.insert(make_pair(id, texture));

	OutputDebugStringW(ToLPCWSTR("[INFO] Texture loaded Ok:  " + filePath + "\n"));
	OutputDebugStringW(ToLPCWSTR("[INFO] Texture ID: " + id + "\n"));
}

void CTextureManager::Add(std::string id, Texture texture) {
	textures[id] = texture;
}

CTextureManager::~CTextureManager()
{
	
}

Texture CTextureManager::Get(String id)
{
	Texture t = textures[id];
	if (t == NULL)
		DebugOut(L"[ERROR] Texture Id %d not found !\n", id);
	
	return t;
}