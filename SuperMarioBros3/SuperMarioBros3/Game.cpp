#include "Game.h"
#include "Ultis.h"
#include "XMLHelper.h"

#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "GameKeyEventHandler.h"

#include "Scene1.h"
#include <string>

#include "Const.h"
#include "WorldMap1.h"
#include "Scene4.h"
#include "Intro.h"

#include <fstream>

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

	// TODO: remove config import
	// Load("config/root.txt");
	Load("Resources/root.xml");

	// Init library helper
	this->d3dHelper = new D3DHelper(hWnd, hInstance);
	this->d3dHelper->InitKeyboardDevice();
	this->SetKeyHandler(new CGameKeyEventHandler());

	DebugOut(L"[INFO] Init Manager \n");
	_Load("config/test.txt");

	auto spriteManager = CSpriteManager::GetInstance();
	spriteManager->LoadSprite(TEXTURE_MARIO, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_MARIO));
	spriteManager->LoadSprite(TEXTURE_FIRE_BALL, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_FIRE_BALL));
	spriteManager->LoadSprite(TEXTURE_REWARD, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_REWARD));
	spriteManager->LoadSprite(TEXTURE_ENEMY, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_ENEMY));
	spriteManager->LoadSprite(TEXTURE_EFFECT, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_EFFECT));
	spriteManager->LoadSprite(TEXTURE_MISC, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_MISC));
	spriteManager->LoadSprite(TEXTURE_UI, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_UI));
	spriteManager->LoadSprite(TEXTURE_WORLD_1, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_WORLD_1));
	spriteManager->LoadSprite(TEXTURE_INTRO, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_INTRO));
	spriteManager->LoadSprite(TEXTURE_PIPE, GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_PIPE));
	
	auto animationManager = CAnimationManager::GetInstance();
	animationManager->InitAnAnimationSet(TEXTURE_MARIO, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_MARIO));
	animationManager->InitAnAnimationSet(TEXTURE_FIRE_BALL, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_FIRE_BALL));
	animationManager->InitAnAnimationSet(TEXTURE_ENEMY, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_ENEMY));
	animationManager->InitAnAnimationSet(TEXTURE_EFFECT, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_EFFECT));
	animationManager->InitAnAnimationSet(TEXTURE_REWARD, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_REWARD));
	animationManager->InitAnAnimationSet(TEXTURE_MISC, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_MISC));
	animationManager->InitAnAnimationSet(TEXTURE_UI, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_UI));
	animationManager->InitAnAnimationSet(TEXTURE_WORLD_1, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_WORLD_1));
	animationManager->InitAnAnimationSet(TEXTURE_INTRO, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_INTRO));
	animationManager->InitAnAnimationSet(TEXTURE_INTRO, GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_INTRO));

	CSceneManager::GetInstance()->Init();
	CIntro* intro = new CIntro();
	CSceneManager::GetInstance()->Load(intro);
	
	// SwitchScene();
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

	Color bgColor = D3DCOLOR_XRGB(0, 0, 0);
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

bool CGame::Load(String path)
{
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

void CGame::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start"){
		next_scene = tokens[1];
	} else if (tokens[0] == "width") {
		this->screenWidth = atoi(tokens[1].c_str());
	} else if (tokens[0] == "height") {
		this->screenHeight = atoi(tokens[1].c_str());
	}
	else
		DebugOut(L"[ERROR] Unknown game setting: %s\n", ToWSTR(tokens[0]).c_str());
}

void CGame::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	String texID = tokens[0].c_str();
	String path = tokens[1];

	CTextureManager::GetInstance()->Add(texID, LoadTexture(path));
}

void CGame::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	String id = tokens[0];
	String path = tokens[1];   // file: ASCII format (single-byte char) => Wide Char

	//LPScene scene = CSceneManager::GetInstance()->Add(id, path);
	//scenes[id] = scene;
}

#define MAX_GAME_LINE 1024
#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2
#define GAME_FILE_SECTION_TEXTURES 3
/*
	Load game campaign file and load/initiate first scene
*/
void CGame::_Load(String gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[TEXTURES]") { section = GAME_FILE_SECTION_TEXTURES; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }
		if (line[0] == '[') 
		{ 
			section = GAME_FILE_SECTION_UNKNOWN; 
			DebugOut(L"[ERROR] Unknown section: %s\n", ToLPCWSTR(line));
			continue; 
		}

		//
		// data section
		//
		switch (section)
		{
			case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
			//case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
			case GAME_FILE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);
}

void CGame::SwitchScene()
{
	if (next_scene == "" || next_scene == current_scene) return; 

	DebugOut(L"[INFO] Switching to scene %s\n", next_scene);

	// scenes[current_scene]->Unload();

	// CSprites::GetInstance()->Clear();
	// CAnimations::GetInstance()->Clear();

	current_scene = next_scene;
	LPScene s = scenes[next_scene];
	//this->SetKeyHandler(s->GetKeyEventHandler());
	// s->Load();

	CSceneManager::GetInstance()->Unload(current_scene);
	CSceneManager::GetInstance()->Load(s);
	// CSceneManager::GetInstance()->SwitchScene(s);
	
}

