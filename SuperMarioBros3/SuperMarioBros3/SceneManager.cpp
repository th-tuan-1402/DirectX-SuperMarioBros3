#include "SceneManager.h"
#include "XMLHelper.h"
#include "Ultis.h"
#include "Game.h"
#include "Const.h"
#include "Intro.h"
#include "Scene1.h"
#include "Scene4.h"
#include "WorldMap1.h"

LPSceneManager CSceneManager::instance = NULL;

CSceneManager::CSceneManager()
{
	currentNodeID = -1;
}

LPSceneManager CSceneManager::GetInstance()
{
	if (instance == NULL) instance = new CSceneManager();
	return instance;
}

void CSceneManager::Init(String filePath)
{
	XMLHelper::forEach(
        filePath,

        // Read config
        [&](XMLElement *ui)
		{
			string name = ui->Attribute("name");
			if (name.compare("UICamera") == 0)
			{
				DebugOut(L"[INFO] Load UI camera \n");
				int screenWidth = CGame::GetInstance()->GetScreenWidth();
				int screenHeight = CGame::GetInstance()->GetScreenHeight();

				Point pos, posHUD;
				ui->QueryFloatAttribute("pos_x", &pos.x);
				ui->QueryFloatAttribute("pos_y", &pos.y);

				XMLElement *uiCam = ui->FirstChildElement();
				std::string nameUICam = uiCam->Attribute("name");

				if (nameUICam.compare("HUD") == 0)
				{
					uiCam->QueryFloatAttribute("pos_x", &posHUD.x);
					uiCam->QueryFloatAttribute("pos_y", &posHUD.y);
					this->uiCamera = new CUICamera(screenWidth, screenHeight, posHUD);
					this->uiCamera->SetPositionCam(pos);
				}
			}
		}
	);
}


//Load Resource
void CSceneManager::Load(LPScene scene)
{
	if (loadedScenes.find(scene->GetSceneId()) != loadedScenes.end())
		loadedScenes.erase(scene->GetSceneId());
	scene->Load(); // Scene Manager nó tự load
	loadedScenes.insert(make_pair(scene->GetSceneId(), scene));
	activeSceneId = scene->GetSceneId();
}

void CSceneManager::Unload(std::string sceneId)
{
	if (loadedScenes.find(sceneId) != loadedScenes.end())
		loadedScenes.at(sceneId)->Unload();
}

LPScene CSceneManager::GetScene(std::string id)
{
	return loadedScenes[id];
}

CUICamera* CSceneManager::GetUICamera()
{
	return uiCamera;
}

std::string CSceneManager::GetActiveSceneId()
{
	return activeSceneId;
}

LPScene CSceneManager::GetActiveScene()
{
	if (activeSceneId == "") return nullptr;
	if (loadedScenes.find(activeSceneId) != loadedScenes.end())
	{
		return loadedScenes.at(activeSceneId);
	}
	return nullptr;
}

int CSceneManager::GetNodeID()
{
	return currentNodeID;
}

void CSceneManager::SetNodeID(int id)
{
	this->currentNodeID = id;
}

void CSceneManager::LoadRequestScene()
{
	if (requestedLoadScene.size() > 0)
	{
		auto scene = requestedLoadScene.at(0);
		if (scene != NULL)
		{
			requestedLoadScene.erase(requestedLoadScene.begin());
			Load(scene);
		}
	}
}

void CSceneManager::UnloadRequestScene()
{
	if (requestedUnloadScene.size() > 0)
	{
		auto scene = requestedUnloadScene.at(0);
		if (scene != NULL)
		{
			requestedUnloadScene.erase(requestedUnloadScene.begin());
			Unload(scene->GetSceneId());
		}
	}
}

void CSceneManager::SwitchScene(LPScene scene)
{
	auto activeScene = GetActiveScene();
	if (activeScene == NULL)
		return;
	requestedUnloadScene.push_back(activeScene);
	requestedLoadScene.push_back(scene);
}

CSceneManager::~CSceneManager()
{
	for (auto s : loadedScenes)
	{
		delete s.second;
	}
	loadedScenes.clear();
}


LPScene CSceneManager::Add(String id, String path) {

	if (id == "intro")
	{
		return new CIntro(path);
	}
	if (id == "world-1")
	{
		return new CWorldMap1(path);
	}
	if (id == "world-1-1")
	{
		return new CScene1(path);
	}
	if (id == "world-1-4")
	{
		return new CScene4(path);
	}

	return nullptr;
}