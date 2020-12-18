﻿#include "Scene.h"
#include "Ultis.h"
#include "tinyxml.h"
#include "Mario.h"
#include "Game.h"
#include "MarioController.h"
#include "SceneConst.h"
#include "Koopa.h"
#include "CVenus.h"
#include "ObjectPool.h"
#include "UICamera.h"

#include <string>
#include "SceneManager.h"
#include "MarioMap.h"
#include "Card.h"
#include "QuestionBlock.h"

using namespace std;

CScene::CScene()
{
	camera = NULL;
	loaded = true;
	cardState = 0;
}

void CScene::Load()
{
	TiXmlDocument sceneFile(filePath.c_str());
	if (!sceneFile.LoadFile())
	{
		DebugOut(L"[ERROR] Cannot load file \n");
		return;
	}
	TiXmlElement* root = sceneFile.RootElement();
	CMarioController* player = NULL;
	for (TiXmlElement* scene = root->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement())
	{
		string name = scene->Attribute("name");
		if (name.compare("Player") == 0)
		{
			DebugOut(L"[INFO] Load player \n");
			D3DXVECTOR2 startPosition;
			scene->QueryFloatAttribute("pos_x", &startPosition.x);
			scene->QueryFloatAttribute("pos_y", &startPosition.y);

			player = new CMarioController();
			player->AddStateObjectsToScene(this);
			player->SetPosition(startPosition);
			player->GetCurrentStateObject()->SetPosition(startPosition);
			AddObject(player);
		}
		if (name.compare("Map") == 0)
		{
			DebugOut(L"[INFO] Load map \n");
			string sourceMap = scene->Attribute("source");
			string fileMap = scene->Attribute("fileName");
			this->map = NULL;
			this->map = new CMap(sourceMap, fileMap); // Ham nay tu load map
			auto tilemap = map->GetTileMap();
			bricks = tilemap->GetBricks();
			coins = tilemap->GetCoins();
			poolBricks = tilemap->GetPoolBricks();
			poolCoins = tilemap->GetPoolCoins();
			poolBricks->AddPoolToScene(this);
			poolCoins->AddPoolToScene(this);
			card = tilemap->GetCard();
			auto mapObjs = map->GetListGameObjects();
			for (auto obj : mapObjs)
			{
				if (obj->GetTag() == GameObjectTags::Enemy)
				{
					auto enemy = static_cast<CEnemy*>(obj);
					if (player != NULL)
					{
						enemy->SetTarget(player);
					}
					if (enemy->GetEnemyTag() == EnemyTag::Venus)
					{
						auto venus = static_cast<CVenus*>(enemy);
						venus->GetObjectPool().AddPoolToScene(this);
					}
				}
				if (obj->GetTag() == GameObjectTags::QuestionBlock)
				{
					auto qB = static_cast<CQuestionBlock*>(obj);
					if (qB != NULL)
						qB->SetTarget(player);
				}
				AddObject(obj);
			}
			DebugOut(L"[INFO] Load background color \n");
			for (TiXmlElement* color = scene->FirstChildElement(); color != NULL; color = color->NextSiblingElement())
			{
				int R, G, B;
				color->QueryIntAttribute("R", &R);
				color->QueryIntAttribute("G", &G);
				color->QueryIntAttribute("B", &B);
				backgroundColor = D3DCOLOR_XRGB(R, G, B);
			}
		}
		else if (name.compare("Camera") == 0)
		{
			DebugOut(L"[INFO] Load camera \n");
			int viewportWidth, viewportHeight, start;
			scene->QueryIntAttribute("start", &start);
			scene->QueryIntAttribute("width", &viewportWidth);
			scene->QueryIntAttribute("height", &viewportHeight);

			this->camera = new CCamera(viewportWidth, viewportHeight);

			for (TiXmlElement* boundary = scene->FirstChildElement(); boundary != NULL; boundary = boundary->NextSiblingElement())
			{
				int id, disX, disY;
				RectF bound;
				D3DXVECTOR2 pos;
				boundary->QueryIntAttribute("id", &id);

				boundary->QueryFloatAttribute("pos_x", &pos.x);
				boundary->QueryFloatAttribute("pos_y", &pos.y);
				boundary->QueryFloatAttribute("left", &bound.left);
				boundary->QueryFloatAttribute("top", &bound.top);
				boundary->QueryFloatAttribute("right", &bound.right);
				boundary->QueryFloatAttribute("bottom", &bound.bottom);

				boundary->QueryIntAttribute("disX", &disX);
				boundary->QueryIntAttribute("disY", &disY);

				camera->AddCameraProperties(id, pos, bound, disX, disY);
				if (start == id)
				{
					camera->SetCurrentBoundary(bound);
					camera->SetPositionCam(pos);
					camera->SetDisablePosX(disX);
					camera->SetDisablePosY(disY);
				}
			}
			if (player != NULL)
			{
				camera->SetGameObject(player->GetCurrentStateObject());
			}
		}
		if (name.compare("Player-Map") == 0)
		{
			DebugOut(L"[INFO] Load player in map\n");
			D3DXVECTOR2 startPosition;
			scene->QueryFloatAttribute("pos_x", &startPosition.x);
			scene->QueryFloatAttribute("pos_y", &startPosition.y);

			CMarioMap* marioMap = new CMarioMap();
			marioMap->SetPosition(startPosition);
			AddObject(marioMap);
		}
	}
	loaded = true;
}

void CScene::Unload()
{
	loaded = false;
	if (gameObjects.size() > 0)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->SetDestroy(true);
		}
	}
}

void CScene::DestroyObject()
{
	if (loaded == false)
	{
		for (auto gO : gameObjects)
		{
			if (gO->IsDestroyed() == true)
			{
				RemoveObject(gO);
				delete gO;
				gO = NULL;
			}
		}
		gameObjects.clear();
		delete map;
		map = NULL;
		camera = NULL;
	}
	if (destroyObjects.size() > 0)
	{
		for (auto gO : destroyObjects)
		{
			RemoveObject(gO);
			delete gO;
			gO = NULL;
		}
		destroyObjects.clear();
	}
}

void CScene::Update(DWORD dt)
{
	if (loaded == false)
		return;
	if (card != NULL && CGame::GetTimeScale() != 0)
	{
		auto cardGO = static_cast<CCard*>(card);
		if (cardGO->IsTouched() == false)
		{
			cardState++;
			if (cardState > 10)
				cardState = 0;
			cardGO->SetCardState(cardState);
		}
		else
		{
			cardGO = NULL;
			card = NULL;
		}
	}
	auto uiCam = CSceneManager::GetInstance()->GetUICamera();
	if (updateObjects.size() == 0) return;
	for (auto obj : updateObjects)
	{
		if (obj->IsIgnoreTimeScale() == false 
			&& CGame::GetTimeScale() == 0)							continue;
		if (obj->IsIgnoreTimeScale() == false
			&& CGame::GetTimeScale() == 0)							continue;
		if (obj->IsEnabled() == false)								continue;
		if (uiCam != NULL)											obj->Update(dt, camera, uiCam);
		else														obj->Update(dt, camera, NULL);
		obj->PhysicsUpdate(&updateObjects);
	}
	if (camera != NULL)
		map->Update(camera, dt);
}

void CScene::Render()
{
	if (loaded == false)
		return;
	map->Render(camera, false);
	if (gameObjects.size() == 0) return;

	for (auto obj : gameObjects)
	{
		if (obj->IsEnabled() == false)								continue;

		obj->Render(camera);
		if (obj->GetCollisionBox()->size() != 0)
			obj->GetCollisionBox()->at(0)->Render(camera, CollisionBox_Render_Distance);
	}
	map->Render(camera, true);
}

void CScene::FindUpdateObjects()
{
	if (gameObjects.size() == 0) return;
	updateObjects.clear();
	for (auto obj : gameObjects)
	{
		if (camera != NULL
			&& camera->CheckObjectInCamera(obj) == false)			continue;
		updateObjects.push_back(obj);
	}
}

void CScene::AddObject(LPGameObject gameObject)
{
	if (gameObject != NULL)
		gameObjects.push_back(gameObject);
}

void CScene::RemoveObject(LPGameObject gameObject)
{
	auto gameObj = find(gameObjects.begin(), gameObjects.end(), gameObject);
	if (gameObj != gameObjects.end())
	{
		gameObjects.erase(gameObj);
	}
}

void CScene::SetObjectPosition(D3DXVECTOR2 distance)
{
	for (auto obj : gameObjects)
	{
		if (obj->GetTag() == GameObjectTags::Solid)
		{
			auto pos = obj->GetCollisionBox()->at(0)->GetPosition();
			obj->GetCollisionBox()->at(0)->SetPosition(pos + distance);
		}
	}
}

std::vector<LPGameObject> CScene::GetBricks()
{
	return bricks;
}

std::vector<LPGameObject> CScene::GetCoins()
{
	return coins;
}

CObjectPool* CScene::GetPoolBricks()
{
	return poolBricks;
}

CObjectPool* CScene::GetPoolCoins()
{
	return poolCoins;
}

void CScene::RemoveBrick(CGameObject* gameObject)
{
	auto gameObj = find(bricks.begin(), bricks.end(), gameObject);
	if (gameObj != bricks.end())
	{
		bricks.erase(gameObj);
	}
}

void CScene::RemoveCoin(CGameObject* gO)
{
	auto gameObj = find(coins.begin(), coins.end(), gO);
	if (gameObj != coins.end())
	{
		coins.erase(gameObj);
	}
}

void CScene::AddBrick(CGameObject* gO)
{
	if (gO != NULL)
		bricks.push_back(gO);
}

void CScene::AddCoin(CGameObject* gO)
{
	if (gO != NULL)
		coins.push_back(gO);
}

void CScene::AddDestroyObject(CGameObject* gO)
{
	if (gO != NULL)
		destroyObjects.push_back(gO);
}

bool CScene::SwitchBlockStateOnToOff()
{
	return switchBlockOffToOn;
}

void CScene::SwitchBlockStateOnToOff(bool state)
{
	this->switchBlockOffToOn = state;
}

void CScene::SetCamera(int id)
{
	auto camProps = camera->GetCameraProperties(id);
	if (CameraPropertieSet::IsEmpty(camProps) == false)
	{
		camera->SetCurrentBoundary(camProps.boundarySet);
		camera->SetPositionCam(camProps.camPosition);
		camera->SetDisablePosX(camProps.disableX);
		camera->SetDisablePosY(camProps.disableY);
	}
}

std::vector<LPGameObject> CScene::GetObjects()
{
	return gameObjects;
}

LPGameObject CScene::GetPlayer()
{
	LPGameObject player = NULL;
	for (auto obj : gameObjects)
		if (obj->GetTag() == GameObjectTags::Player) 
			player = obj;
	return player;
}

bool CScene::IsLoaded()
{
	return loaded;
}

CScene::~CScene()
{
}
