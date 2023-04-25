
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
	auto root = CGame::GetInstance();
	LoadTexture(TEXTURE_MARIO, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_MARIO));
	LoadTexture(TEXTURE_BBOX, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_BBOX));
	LoadTexture(TEXTURE_MISC, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_MISC));
	LoadTexture(TEXTURE_REWARD, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_REWARD));
	LoadTexture(TEXTURE_FIRE_BALL, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_FIRE_BALL));
	LoadTexture(TEXTURE_ENEMY, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_ENEMY));
	LoadTexture(TEXTURE_EFFECT, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_EFFECT));
	LoadTexture(TEXTURE_UI, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_UI));
	LoadTexture(TEXTURE_BLACK, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_BLACK));
	LoadTexture(TEXTURE_WORLD_1, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_WORLD_1));
	LoadTexture(TEXTURE_INTRO, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_INTRO));
	LoadTexture(TEXTURE_PIPE, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_PIPE));
}

void CTextureManager::Clear()
{
	for (auto t : textures)
	{
		Texture tex = t.second;
		//Todo: clear when refactor
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
		//Todo: clear when refactor
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

CTextureManager::~CTextureManager()
{
	
}
