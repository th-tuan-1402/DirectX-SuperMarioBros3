#include "XMLHelper.h"
#include "Ultis.h"
#include "Game.h"

#include "AnimationManager.h"
#include "SpriteManager.h"
#include "GraphicConst.h"
#include "Const.h"

LPAnimationManager CAnimationManager::instance = NULL;

LPAnimationManager CAnimationManager::GetInstance()
{
    if (instance == NULL) instance = new CAnimationManager();
    return instance;
}

CAnimationManager::~CAnimationManager()
{
	
}

CAnimationManager::CAnimationManager()
{
}

bool CAnimationManager::Init()
{
	return true;
}

bool CAnimationManager::InitAnAnimationSet(String textureName, String filePath)
{
	if (LoadAnimation(textureName, filePath) == false)
	{
		DebugOut(L"[ERROR] Cannot Load", textureName + "Animation File \n");
		return false;
	}
	return true;
}

bool CAnimationManager::LoadAnimation(String texName, String filePath)
{
	// Load from XML
	return XMLHelper::forEach(
		filePath,

		// Read config
		[&](XMLElement *element)
			{
				String gameObjectID = element->Attribute("gameObjectId");
				String textureID    = element->Attribute("textureId");

				if (textureID == texName) {

					Texture tex = CTextureManager::GetInstance()->GetTexture(textureID);

					if (tex != nullptr)
						OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));
					else
						return false;

					OutputDebugStringW(ToLPCWSTR("Gameobject id: " + gameObjectID + '\n'));
					OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

					for (XMLElement* node = element->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
					{
						String aniId = node->Attribute("aniId");
						int frameTime;
						node->QueryIntAttribute("frameTime", &frameTime);
						String name = node->Attribute("name");
						
						OutputDebugStringW(ToLPCWSTR(aniId + ':' + to_string(frameTime) + ':' + name + '\n'));
						LPAnimation animation = new CAnimation(aniId, frameTime);

						// Sprite ref
						for (XMLElement* sprNode = node->FirstChildElement(); sprNode != nullptr; sprNode = sprNode->NextSiblingElement())
						{
							string id = sprNode->Attribute("id");
							LPSprite sprite = CSpriteManager::GetInstance()->Get(id);
							animation->Add(sprite, frameTime);
							OutputDebugStringW(ToLPCWSTR("|--" + id + ':' + to_string(frameTime) + '\n'));
						}

						AddAnimation(aniId, animation);
					}
				}
			}
	);
}

void CAnimationManager::AddAnimation(String id, LPAnimation ani)
{
	animations.insert(make_pair(id, ani));
}

LPAnimation CAnimationManager::Get(String id)
{
	LPAnimation ani = animations.at(id);
	if (ani == NULL)
	{
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
		return nullptr;
	}	
	return ani;
}

LPAnimation CAnimationManager::Clone(String id)
{
	LPAnimation oriAni = Get(id);
	LPAnimation copiedAni = new CAnimation(*oriAni);
	return copiedAni;
}


void CAnimationManager::Clear()
{
	for (auto x : animations)
	{
		LPAnimation ani = x.second;
		delete ani;
	}
	animations.clear();
}

