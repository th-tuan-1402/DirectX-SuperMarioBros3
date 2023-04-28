#include "WorldMap1.h"
#include "Game.h"
#include "Const.h"
#include "SceneManager.h"
CWorldMap1::CWorldMap1(String path): CScene(path)
{
	this->id = SC_WOLRD_1;
	this->filePath = "Resources/Scenes/World1.xml";
}

void CWorldMap1::Load()
{
	spaceParitioning = false;
	CScene::Load();
	CSceneManager::GetInstance()->GetUICamera()->SetDisableBlackTexture(true);
}

void CWorldMap1::Unload()
{
	CScene::Unload();
}

CWorldMap1::~CWorldMap1()
{
}
