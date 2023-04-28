#include "Intro.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"
#include "MarioConst.h"
#include "Game.h"
#include "SceneManager.h"
#include "IntroScript.h"
CIntro::CIntro(String path): CScene(path)
{
	this->id = SC_INTRO;
	this->filePath = "Resources/Scenes/Intro.xml";
	spaceParitioning = false;
}

void CIntro::Load()
{
	CScene::Load();
	CSceneManager::GetInstance()->GetUICamera()->SetDisableBlackTexture(true);
	CSceneManager::GetInstance()->GetUICamera()->SetEnterIntro(true);
	CIntroScript* introScript = new CIntroScript();
	AddObject(introScript);
}

void CIntro::Unload()
{
	CSceneManager::GetInstance()->GetUICamera()->SetEnterIntro(false);
	CScene::Unload();
}
