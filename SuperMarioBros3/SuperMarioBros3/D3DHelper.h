#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>

#include "Type.h"
#include "Ultis.h"

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

class D3DHelper
{
private:

	HWND hWnd; // Window Handle

    LPDIRECT3D9 d3d = NULL; // Direct3D handle
    LPDIRECT3DDEVICE9 d3ddv = NULL; // Direct3D device handle

    /* Draw */
	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL; // Sprite helper libar

    /* Input */
    LPDIRECTINPUT8       dInput;		// The DirectInput object
	LPDIRECTINPUTDEVICE8 dInputDevice;  // Device có thể là là bất kỳ thiết bị nào. Nhưng trg game của mình thì chỉ có keyboard nên đây là Keyboard device 

public:
    // Constuctor
    D3DHelper(HWND hWnd, int screenWidth, int screenHeight) {
        this->hWnd = hWnd;
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

    //==========================================
    // Direct input
    //==========================================
    void InitKeyboardDevice()
    {
        HRESULT
            hr = DirectInput8Create
            (
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                DIRECTINPUT_VERSION,
                IID_IDirectInput8, (VOID**)&dInput, NULL
            );
        if (hr != DI_OK)
        {
            DebugOut(L"[ERROR] DirectInput8 Create failed!\n");
            return;
        }

        hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputDevice, NULL);
        if (hr != DI_OK)
        {
            DebugOut(L"[ERROR] CreateDevice failed!\n");
            return;
        }

        // Set the data format to "keyboard format" - a predefined data format 
        //
        // A data format specifies which controls on a device we
        // are interested in, and how they should be reported.
        //
        // This tells DirectInput that we will be passing an array
        // of 256 bytes to IDirectInputDevice::GetDeviceState.

        hr = dInputDevice->SetDataFormat(&c_dfDIKeyboard);

        hr = dInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


        // IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
        //
        // DirectInput uses unbuffered I/O (buffer size = 0) by default.
        // If you want to read buffered data, you need to set a nonzero
        // buffer size.
        //
        // Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
        //
        // The buffer size is a DWORD property associated with the device.
        DIPROPDWORD dipdw;

        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj = 0;
        dipdw.diph.dwHow = DIPH_DEVICE;
        dipdw.dwData = KEYBOARD_BUFFER_SIZE;

        hr = dInputDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

        hr = dInputDevice->Acquire();
        if (hr != DI_OK)
        {
            DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
            return;
        }

        DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
    }

    LPDIRECTINPUTDEVICE8 GetInputDevice() { return this->dInputDevice; }
};
