#include "Game.h"
#include "GameConst.h"
#include "Ultis.h"
#include "XMLHelper.h"

#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "GameKeyEventHandler.h"

#include "Scene1.h"
#include <string>
#include "WorldMap1.h"
#include "Scene4.h"
#include "Intro.h"

CGame* CGame::instance = NULL;
DWORD CGame::deltaTime = 0;
float CGame::timeScale = 1.0f;

CGame* CGame::GetInstance()
{
    if (instance == NULL) instance = new CGame();
    return instance;
}

CGame::~CGame() 
{
}

void CGame::Init(HWND hWnd, HINSTANCE hInstance)
{
	this->hWnd = hWnd;

	// Todo: remove config import
	ImportGameSource();

	// Init library helper
	this->d3dHelper = new D3DHelper(hWnd, hInstance);
	this->d3dHelper->InitKeyboardDevice();
	this->SetKeyHandler(new CGameKeyEventHandler());

	this->screenWidth = 720;
	this->screenHeight = 610;

	DebugOut(L"[INFO] Init Manager \n");
	CTextureManager::GetInstance()->Init();
	CSpriteManager::GetInstance()->Init();
	CAnimationManager::GetInstance()->Init();
	CSceneManager::GetInstance()->Init();
	
	CIntro* intro = new CIntro();
	CSceneManager::GetInstance()->Load(intro);
	DebugOut(L"[INFO] Init Manager Sucessfully \n");
}

void CGame::Request()
{
	CSceneManager::GetInstance()->LoadRequestScene();
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene != nullptr)
	{
		activeScene->FindUpdateObjects();
	}
}

void CGame::Draw(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor)
{
	this->d3dHelper->Draw(position.x, position.y, texture, &rect, transcolor, 0, 0);
}

void CGame::Draw(Point position, Texture texture, RECT rect, int alpha)
{
	this->d3dHelper->Draw(position.x, position.y, texture, &rect, (FLOAT)alpha, 0, 0);
}

void CGame::DrawFlipX(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor)
{
	this->d3dHelper->Draw(position.x, position.y, texture, &rect, transcolor, 0, 0);
}

void CGame::DrawFlipY(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor)
{
	this->d3dHelper->Draw(position.x, position.y, texture, &rect, transcolor, 0, 0);
}

void CGame::Run(DWORD deltaTime)
{
	this->deltaTime = deltaTime;

	Request();

	// Process key
	// auto keyboardManger = CKeyboardManager::GetInstance();
	ProcessKeyboard();

	if (CheckESCKey() == false)
	{
		Update();
		Render();
		Clean();
	}
}

void CGame::End()
{
	DebugOut(L"[INFO] This game is about to end \n");

	CSceneManager::GetInstance()->GetActiveScene()->Unload();
	CSceneManager::GetInstance()->GetActiveScene()->DestroyObject();
	CTextureManager::GetInstance()->Clear();
	CSpriteManager::GetInstance()->Clear();
	CAnimationManager::GetInstance()->Clear();

	this->d3dHelper->Release();
	DebugOut(L"[INFO] Bye bye \n");
}

void CGame::Clean()
{
	CSceneManager::GetInstance()->UnloadRequestScene();
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene != nullptr)
		activeScene->DestroyObject();
}

void CGame::Render()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	auto uiCamera = CSceneManager::GetInstance()->GetUICamera();

	D3DCOLOR bgColor = D3DCOLOR_XRGB(0, 0, 0);
	if (activeScene != nullptr)
		bgColor = activeScene->GetBackgroundColor();

	this->d3dHelper->PlayScene(bgColor);
	{
		if (activeScene != nullptr)
			activeScene->Render();
		if (uiCamera != nullptr)
			uiCamera->Render();
	}
	this->d3dHelper->StopScene();
}

void CGame::Update()
{
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	auto uiCamera = CSceneManager::GetInstance()->GetUICamera();
	// Update Scene. Trong Scene sẽ Update các GameObject. Trong GameObject sẽ update các animation. Các animation sẽ update các animation frame / sprite ?
	if (activeScene != NULL)
		activeScene->Update(deltaTime);
	if (uiCamera != nullptr)
		uiCamera->Update();
}

bool CGame::ImportGameSource()
{
	String path = "Resources/root.xml";

	return XMLHelper::forEach(
        path,

        // Read config
        [&](XMLElement *element)
            {
				std::string category = element->Attribute("name");
				OutputDebugStringW(ToLPCWSTR(category + '\n'));

				std::unordered_map<std::string, std::string> bucket;

				for (auto item = element->FirstChildElement(); item != nullptr; item = item->NextSiblingElement())
				{
					std::string id = item->Attribute("id");
					std::string source = item->Attribute("source");
					bucket.insert(make_pair(id, source)); 
					OutputDebugStringW(ToLPCWSTR("|--" + id + ":" + source + '\n'));
				}
				
				gameSource.insert(make_pair(category, bucket));
			}
	);
}

String CGame::GetFilePathByCategory(String category, String id)
{
	if (gameSource.find(category) != gameSource.end())
	{
		auto bucket = gameSource.at(category);
		if (bucket.find(id) != bucket.end())
		{
			return bucket.at(id);
		}
		return "";
	}
	return "";
}

void CGame::ProcessKeyboard()
{
	// CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	// Nếu mario đang chui vào pipe_gate hoặc chui ra khỏi pipe_des để về mặt đất trong scene 1-1
	// Thì không có xử lý keyboard gì hết ớ, player bây giờ không có cái quyền điều khiên gì hết ớ
	//
	// Trừ khi game có chức năng Pause Game, nếu không thì code như thế này là ổn rồi
	// if (mario->IsGoingIntoPipeGate() || mario->IsGettingOutOfPipeDes() || mario->IsTailAttacking())
	// 	return;

	HRESULT hr;
	auto didv = d3dHelper->GetInputDevice();

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else
				return;
		}
		else
		{
			// DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);

		keyHandler->KeyState();
	}
}

bool CGame::CheckESCKey()
{
	// TODO: Cần chỉnh lại vì nút ESC Chỉ cần nhấn 1 lần
	if (this->IsKeyDown(DIK_ESCAPE))
	{
		DebugOutTitle(L"Nhan ESC");
		DebugOut(L"Nhan ESC \n");
		CGame::GetInstance()->End();
		PostQuitMessage(0);
		return true;
	}
	return false;
}

bool CGame::IsKeyDown(int keyCode)
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene->GetSceneId() == "intro")
	{
		if (keyCode != DIK_Q && keyCode != DIK_W)
			return false;
	}
	return (keyStates[keyCode] & 0x80) > 0; // Lấy ra nút được ấn
}

bool CGame::IsKeyUp(int keyCode)
{
	return (keyStates[keyCode] & 0x80) <= 0;
}