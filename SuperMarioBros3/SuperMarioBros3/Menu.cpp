#include "Menu.h"
#include "Ultis.h"
#include "SpriteManager.h"
#include <dinput.h>
#include "WorldMap1.h"
#include "SceneManager.h"

CMenu::CMenu()
{
	tag = GameObjectTags::Menu;
	selection1 = new CFont();
	auto pos = Point(250,440);
	selection1->SetCurrentText("1 PLAYER GAME");
	selection1->SetPosition(pos);

	pos.y += 50;
	selection2 = new CFont();
	selection2->SetCurrentText("2 PLAYER GAME");
	selection2->SetPosition(pos);

	auto spriteManager = CSpriteManager::GetInstance();
	arrow = spriteManager->Get("spr-menu-arrow-0");
	treeLeft = spriteManager->Get("spr-tree-left");
	treeRight = spriteManager->Get("spr-tree-right");
	bottomCurtain = spriteManager->Get("spr-bottom-curtain-0");
	isEnabled = true;

	direction.insert(make_pair("up", Point(200, 440)));
	direction.insert(make_pair("down", Point(200, 490)));
	currentDirection = direction.at("up");
}

void CMenu::Render(CCamera* cam, int alpha)
{
	selection1->Render();
	selection2->Render();
	treeLeft->Draw(Point(50, 433) - cam->GetPositionCam(), Point(1, 1), 0);
	treeRight->Draw(Point(680, 384) - cam->GetPositionCam(), Point(1, 1), 0);
	bottomCurtain->Draw(Point(150, 20) - cam->GetPositionCam(), Point(1, 1), 0);
	bottomCurtain->Draw(Point(500, 20) - cam->GetPositionCam(), Point(1, 1), 0);
	arrow->Draw(currentDirection, Point(1.0f, 1.0f), 0.0f);
}

void CMenu::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
		case DIK_DOWN:
		{
			currentDirection = direction.at("down");
			break;
		}
		case DIK_UP:
		{
			currentDirection = direction.at("up");
			break;
		}
		case DIK_Q:
		{
			if (currentDirection == direction.at("up"))
				currentDirection = direction.at("down");
			else
				currentDirection = direction.at("up");
			break;
		}
		case DIK_W:
		{
			auto gameObj = CGame::GetInstance();
			gameObj->SetNextScene("world-1-1");
			gameObj->SwitchScene();
			break;
		}
	}
}

void CMenu::OnKeyUp(int KeyCode)
{
}
