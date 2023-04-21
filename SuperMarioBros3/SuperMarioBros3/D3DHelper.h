#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "Type.h"
#include "Ultis.h"

class D3DHelper
{
private:

    LPDIRECT3D9 d3d = NULL; // Direct3D handle
    LPDIRECT3DDEVICE9 d3ddv = NULL; // Direct3D device handle

    /*Draw*/
	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL; // Sprite helper libar

public:

    LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

    // Constuctor
    D3DHelper(HWND hWnd, int screenWidth, int screenHeight) {
        d3d = Direct3DCreate9(D3D_SDK_VERSION);

        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.BackBufferWidth = screenWidth;
        d3dpp.BackBufferHeight = screenHeight;
        d3dpp.Flags = 0;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
        d3dpp.MultiSampleQuality = NULL;
        d3dpp.BackBufferCount = 1;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.Windowed = true;
        d3dpp.hDeviceWindow = hWnd;

        d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &d3dpp, &d3ddv);

        if (d3ddv == NULL)
        {
            OutputDebugString(L"[ERROR] CreateDevice failed\n");
            return;
        }

        d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

        
        D3DXCreateSprite(d3ddv, &spriteHandler);

        if (!spriteHandler)
        {
            MessageBox(hWnd, L"Creating sprite handler failed!", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
    }

    // Load texture
    Texture LoadTexture(std::string filePath, D3DCOLOR transparentColor)
    {
        Texture texture = NULL;

        D3DXIMAGE_INFO info;
        HRESULT result = D3DXGetImageInfoFromFile(ToLPCWSTR(filePath), &info);
        if (result == D3D_OK) {
            D3DXCreateTextureFromFileEx(
                        d3ddv,								// Pointer to Direct3D device object
                        ToLPCWSTR(filePath),							// Path to the image to load
                        info.Width,							// Texture width
                        info.Height,						// Texture height
                        1,
                        D3DUSAGE_DYNAMIC,
                        D3DFMT_UNKNOWN,
                        D3DPOOL_DEFAULT,
                        D3DX_DEFAULT,
                        D3DX_DEFAULT,
                        transparentColor,
                        &info,
                        NULL,
                        &texture
                    );								// Created texture pointer
        }

        return texture;
    }

    // Draw texture
    void Draw(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255,0,255))
    {
        Point3D pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
        Point3D pInt((int)(position.x), (int)(position.y), 0); // Giúp không bị viền
        spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
    }
	void Draw(Point position, Texture texture, RECT rect, int alpha = 253)
    {
        Point3D pInt((int)(position.x), (int)(position.y), 0); // Giúp không bị viền
	    spriteHandler->Draw(texture, &rect, NULL, &pInt, D3DCOLOR_ARGB(alpha, 255, 255, 255));
    }
	void DrawFlipX(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255))
    {
        Point3D pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
        Point pScale(-1, 1);
        Point3D pInt((int)(position.x), (int)(position.y), 0);
        Matrix oldMatrix, newMatrix; 

        spriteHandler->GetTransform(&oldMatrix);

        D3DXMatrixTransformation2D(&newMatrix, &position, 0.0f, &pScale, NULL, 0.0f, NULL);
        spriteHandler->SetTransform(&newMatrix);

        spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
        spriteHandler->SetTransform(&oldMatrix);
    }
	void DrawFlipY(Point position, Point pointCenter, Texture texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255))
    {
        Point3D pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
        Point pScale(1, -1);
        Point3D pInt((int)(position.x), (int)(position.y), 0);
        Matrix oldMatrix, newMatrix;
        
        spriteHandler->GetTransform(&oldMatrix);

        D3DXMatrixTransformation2D(&newMatrix, &position, 0.0f, &pScale, NULL, 0.0f, NULL);
        spriteHandler->SetTransform(&newMatrix);

        spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
        spriteHandler->SetTransform(&oldMatrix);
    }

    // Control scene
    void PlayScene(D3DCOLOR bgColor)
    {
        d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, bgColor, 1.0f, 0);
        d3ddv->BeginScene();

        spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
    }
    void StopScene()
    {
        spriteHandler->End();

        d3ddv->EndScene();
        d3ddv->Present(NULL, NULL, NULL, NULL);
    }

    
    // Release resource
    void Release() {
        if (spriteHandler != NULL) {
            spriteHandler->Release();
        }
        
        if (backBuffer != NULL) {
            backBuffer->Release();
        }
        
        if (d3ddv != NULL) {
            d3ddv->Release();
        }
        
        if (d3d != NULL) {
            d3d->Release();
        }
    }
};
