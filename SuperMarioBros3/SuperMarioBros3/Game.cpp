#include "Game.h"
#include "GameConst.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "GameKeyEventHandler.h"

#include "Scene1.h"
#include "tinyxml.h"
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

void CGame::Init(HWND hWnd, int scrWidth, int scrHeight, int fps)
{
	// Init library helper
	this->d3dHelper = new D3DHelper(hWnd, scrWidth, scrHeight);

	this->fps = fps;
	this->hWnd = hWnd;
	this->screenWidth = scrWidth;
	this->screenHeight = scrHeight;

	DebugOut(L"[INFO] Init Manager \n");
	CTextureManager::GetInstance()->Init();
	CSpriteManager::GetInstance()->Init();
	CAnimationManager::GetInstance()->Init();
	CSceneManager::GetInstance()->Init();

	CGameKeyEventHandler *keyEventHandler = new CGameKeyEventHandler();
	auto keyboardManager = CKeyboardManager::GetInstance();
	keyboardManager->SetHWND(hWnd);
	keyboardManager->InitKeyboard(keyEventHandler);
	
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
	this->d3dHelper->Draw(position, pointCenter, texture, rect, transcolor);
}

void CGame::Draw(Point position, Texture texture, RECT rect, int alpha)
{
	this->d3dHelper->Draw(position, texture, rect, alpha);
}

void CGame::DrawFlipX(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor)
{
	this->d3dHelper->DrawFlipX(position, pointCenter, texture, rect, transcolor);
}

void CGame::DrawFlipY(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor)
{
	this->d3dHelper->DrawFlipY(position, pointCenter, texture, rect, transcolor);
}

void CGame::Run()
{
	MSG msg;
	bool done = false;

	DWORD frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / fps;

	// Game Loop
	while (!done)
	{
		// Do trong game không có thời gian thực sự, để tạo cảm giác thời gian trôi qua thì ta chỉ có bộ đếm tick từ lúc start game để tạo cảm giác chân thực

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD currentTime = GetTickCount64(); // now
			deltaTime = currentTime - frameStart;

			if (deltaTime >= tickPerFrame) // chuyển frame mới
			{
				frameStart = currentTime;
				Request();
				// Process key
				auto keyboardManger = CKeyboardManager::GetInstance();
				keyboardManger->ProcessKeyboard();
				if (keyboardManger->CheckESCKey() == true)
					continue;
				Update();
				Render();
				Clean();
				
				if (deltaTime > tickPerFrame) deltaTime = 0;
			}
			else // chưa tới tickperframe nên cho ngủ vì xong việc cho 1 frame ròi
			{
				Sleep(tickPerFrame - deltaTime);
			}
		}
		
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
	auto path = "Resources/root.xml";
	TiXmlDocument doc(path);
	if (doc.LoadFile() == false)
	{
		OutputDebugStringW(ToLPCWSTR(doc.ErrorDesc()));
		return false;
	}
	TiXmlElement* root = doc.RootElement();
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
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
	return true;
}

std::string CGame::GetFilePathByCategory(std::string category, std::string id)
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
