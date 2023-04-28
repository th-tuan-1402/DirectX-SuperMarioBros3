#include "Scene1.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"
#include "MarioConst.h"
#include "Game.h"
#include "SceneManager.h"

CScene1::CScene1(String path): CScene(path)
{
	this->id = "PlayScene";
	this->filePath = "Resources/Scenes/World1-1.xml";
}

void CScene1::Load()
{
	CScene::Load();
	CSceneManager::GetInstance()->GetUICamera()->SetDisableBlackTexture(false);
	auto timer = CSceneManager::GetInstance()->GetUICamera()->GetHUD()->GetTimer();
	timer->StartTimer(); // Mỗi scene khi load đều start timer trừ map selection
	DebugOut(L"[INFO] Loaded Scene \n");
}

void CScene1::Unload()
{
	CScene::Unload();
	auto timer = CSceneManager::GetInstance()->GetUICamera()->GetHUD()->GetTimer();
	timer->ResetTimer();
}

CScene1::~CScene1()
{
}
