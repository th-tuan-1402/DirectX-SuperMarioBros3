#pragma once
#include "D3DType.h"
#include <dinput.h>
#include "Type.h"
#include "Ultis.h"


#define KEYBOARD_BUFFER_SIZE 1024

class D3DHelper
{
private:
    // Window Handle
	HWND hWnd;

    // Backbuffer width & height, will be set during Direct3D initialization
    int backBufferWidth = 0;
    int backBufferHeight = 0;

    //============================ D3D10 =================================
    ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state

	SpriteHandler spriteHandler;						// Sprite handling object, BIG MYSTERY: it has to be in this place OR will lead to access violation in D3D11.dll ????
    ID3D10SamplerState* pPointSamplerState;

    /* Input */
    LPDIRECTINPUT8       dInput;		// The DirectInput object
	LPDIRECTINPUTDEVICE8 dInputDevice;  // Device có thể là là bất kỳ thiết bị nào. Nhưng trg game của mình thì chỉ có keyboard nên đây là Keyboard device 

public:
    // Constuctor
    D3DHelper(HWND hWnd, HINSTANCE hInstance)
    {
        this->hWnd = hWnd;

        // Retrieve client area width & height so that we can create backbuffer height & width accordingly
        RECT r;
        GetClientRect(hWnd, &r);

        backBufferWidth = r.right + 1;
        backBufferHeight = r.bottom + 1;

        DebugOut(L"[INFO] Window's client area: width= %d, height= %d\n", r.right - 1, r.bottom - 1);

        // Create & clear the DXGI_SWAP_CHAIN_DESC structure
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

        // Fill in the needed values
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = backBufferWidth;
        swapChainDesc.BufferDesc.Height = backBufferHeight;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hWnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = TRUE;

        // Create the D3D device and the swap chain
        HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
            D3D10_DRIVER_TYPE_REFERENCE,
            NULL,
            0,
            D3D10_SDK_VERSION,
            &swapChainDesc,
            &pSwapChain,
            &pD3DDevice);

        if (hr != S_OK)
        {
            DebugOut((wchar_t *)L"[ERROR] D3D10CreateDeviceAndSwapChain has failed %s %d", _W(__FILE__), __LINE__);
            return;
        }

        // Get the back buffer from the swapchain
        ID3D10Texture2D *pBackBuffer;
        hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID *)&pBackBuffer);
        if (hr != S_OK)
        {
            DebugOut((wchar_t *)L"[ERROR] pSwapChain->GetBuffer has failed %s %d", _W(__FILE__), __LINE__);
            return;
        }

        // create the render target view
        hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

        pBackBuffer->Release();
        if (hr != S_OK)
        {
            DebugOut((wchar_t *)L"[ERROR] CreateRenderTargetView has failed %s %d", _W(__FILE__), __LINE__);
            return;
        }

        // set the render target
        pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

        // create and set the viewport
        D3D10_VIEWPORT viewPort;
        viewPort.Width = backBufferWidth;
        viewPort.Height = backBufferHeight;
        viewPort.MinDepth = 0.0f;
        viewPort.MaxDepth = 1.0f;
        viewPort.TopLeftX = 0;
        viewPort.TopLeftY = 0;
        pD3DDevice->RSSetViewports(1, &viewPort);

        D3D10_SAMPLER_DESC desc;
        desc.Filter = D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        desc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
        desc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
        desc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
        desc.MipLODBias = 0;
        desc.MaxAnisotropy = 1;
        desc.ComparisonFunc = D3D10_COMPARISON_NEVER;
        desc.BorderColor[0] = 1.0f;
        desc.BorderColor[1] = 1.0f;
        desc.BorderColor[2] = 1.0f;
        desc.BorderColor[3] = 1.0f;
        desc.MinLOD = -FLT_MAX;
        desc.MaxLOD = FLT_MAX;

        pD3DDevice->CreateSamplerState(&desc, &this->pPointSamplerState);

        // Create the sprite object to handle sprite drawing
        hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteHandler);

        if (hr != S_OK)
        {
            DebugOut((wchar_t *)L"[ERROR] D3DX10CreateSprite has failed %s %d", _W(__FILE__), __LINE__);
            return;
        }

        Matrix matProjection;

        // Create the projection matrix using the values in the viewport
        D3DXMatrixOrthoOffCenterLH(&matProjection,
            (float)viewPort.TopLeftX,
            (float)viewPort.Width,
            (float)viewPort.TopLeftY,
            (float)viewPort.Height,
            0.1f,
            10);
        hr = spriteHandler->SetProjectionTransform(&matProjection);

        // Initialize the blend state for alpha drawing
        D3D10_BLEND_DESC StateDesc;
        ZeroMemory(&StateDesc, sizeof(D3D10_BLEND_DESC));
        StateDesc.AlphaToCoverageEnable = FALSE;
        StateDesc.BlendEnable[0] = TRUE;
        StateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
        StateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
        StateDesc.BlendOp = D3D10_BLEND_OP_ADD;
        StateDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
        StateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
        StateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
        StateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
        pD3DDevice->CreateBlendState(&StateDesc, &this->pBlendStateAlpha);

        DebugOut((wchar_t*)L"[INFO] InitDirectX has been successful\n");
    };

    /*
        Utility function to wrap D3DXCreateTextureFromFileEx
    */
    Texture LoadTexture(String texturePath)
    {
        ID3D10Resource* pD3D10Resource = NULL;
        ID3D10Texture2D* tex = NULL;

        // Retrieve image information first 
        D3DX10_IMAGE_INFO imageInfo;
        HRESULT hr = D3DX10GetImageInfoFromFile(texturePath, NULL, &imageInfo, NULL);
        if (FAILED(hr))
        {
            DebugOut((wchar_t *)L"[ERROR] D3DX10GetImageInfoFromFile failed for  file: %s with error: %d\n", texturePath, hr);
            return NULL;
        }

        D3DX10_IMAGE_LOAD_INFO info;
        ZeroMemory(&info, sizeof(D3DX10_IMAGE_LOAD_INFO));
        info.Width = imageInfo.Width;
        info.Height = imageInfo.Height;
        info.Depth = imageInfo.Depth;
        info.FirstMipLevel = 0;
        info.MipLevels = 1;
        info.Usage = D3D10_USAGE_DEFAULT;
        info.BindFlags = D3DX10_DEFAULT;
        info.CpuAccessFlags = D3DX10_DEFAULT;
        info.MiscFlags = D3DX10_DEFAULT;
        info.Format = imageInfo.Format;
        info.Filter = D3DX10_FILTER_NONE;
        info.MipFilter = D3DX10_DEFAULT;
        info.pSrcInfo = &imageInfo;

        // Loads the texture into a temporary ID3D10Resource object
        hr = D3DX10CreateTextureFromFile(pD3DDevice,
            texturePath,
            &info,
            NULL,
            &pD3D10Resource,
            NULL);

        // Make sure the texture was loaded successfully
        if (FAILED(hr))
        {
            DebugOut((wchar_t*)L"[ERROR] Failed to load texture file: %s with error: %d\n", texturePath, hr);
            return NULL;
        }

        // Translates the ID3D10Resource object into a ID3D10Texture2D object
        pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&tex);
        pD3D10Resource->Release();

        if (!tex) {
            DebugOut((wchar_t*)L"[ERROR] Failed to convert from ID3D10Resource to ID3D10Texture2D \n");
            return NULL;
        }

        //
        // Create the Share Resource View for this texture 
        //
        // Get the texture details
        D3D10_TEXTURE2D_DESC desc;
        tex->GetDesc(&desc);

        // Create a shader resource view of the texture
        D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;

        // Clear out the shader resource view description structure
        ZeroMemory(&SRVDesc, sizeof(SRVDesc));

        // Set the texture format
        SRVDesc.Format = desc.Format;

        // Set the type of resource
        SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MipLevels = desc.MipLevels;

        ID3D10ShaderResourceView* gSpriteTextureRV = NULL;

        pD3DDevice->CreateShaderResourceView(tex, &SRVDesc, &gSpriteTextureRV);

        DebugOut(L"[INFO] Texture loaded Ok from file: %s \n", texturePath);

        return new CTexture(tex, gSpriteTextureRV);
    }

    // Draw texture
    /*
        Draw the whole texture or part of texture onto screen
        NOTE: This function is very inefficient because it has to convert
        from texture to sprite every time we need to draw it
    */
    void Draw(float x, float y, Texture tex, RECT *rect, float alpha, int sprite_width, int sprite_height)
    {
        if (tex == NULL) return;

        int spriteWidth = sprite_width;
        int spriteHeight = sprite_height;

        D3DX10_SPRITE sprite;

        // Set the sprite’s shader resource view
        sprite.pTexture = tex->getShaderResourceView();

        if (rect == NULL)
        {
            // top-left location in U,V coords
            sprite.TexCoord.x = 0;
            sprite.TexCoord.y = 0;

            // Determine the texture size in U,V coords
            sprite.TexSize.x = 1.0f;
            sprite.TexSize.y = 1.0f;

            if (spriteWidth == 0)
                spriteWidth = tex->getWidth();
            if (spriteHeight == 0)
                spriteHeight = tex->getHeight();
        }
        else
        {
            sprite.TexCoord.x = rect->left / (float)tex->getWidth();
            sprite.TexCoord.y = rect->top / (float)tex->getHeight();

            if (spriteWidth == 0)
                spriteWidth = (rect->right - rect->left + 1);
            if (spriteHeight == 0)
                spriteHeight = (rect->bottom - rect->top + 1);

            sprite.TexSize.x = spriteWidth / (float)tex->getWidth();
            sprite.TexSize.y = spriteHeight / (float)tex->getHeight();
        }

        // Set the texture index. Single textures will use 0
        sprite.TextureIndex = 0;

        // The color to apply to this sprite, full color applies white.
        // sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

        //
        // Build the rendering matrix based on sprite location
        //

        // The translation matrix to be created
        Matrix matTranslation;

        // Create the translation matrix
        D3DXMatrixTranslation(&matTranslation, x, (backBufferHeight - y), 0.1f);

        // Scale the sprite to its correct width and height because by default, DirectX draws it with width = height = 1.0f
        Matrix matScaling;
        D3DXMatrixScaling(&matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);

        // Setting the sprite’s position and size
        sprite.matWorld = (matScaling * matTranslation);

        spriteHandler->DrawSpritesImmediate(&sprite, 1, 0, 0);
    }

    // Control scene
    void PlayScene(D3DXCOLOR backgroudColor)
    {
        pD3DDevice->ClearRenderTargetView(pRenderTargetView, backgroudColor);

        spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

        FLOAT NewBlendFactor[4] = {0, 0, 0, 0};
        pD3DDevice->OMSetBlendState(pBlendStateAlpha, NewBlendFactor, 0xffffffff);
    }
    void StopScene()
    {
        spriteHandler->End();
        pSwapChain->Present(0, 0);
    }

    // Release resource
    void Release()
    {
        pBlendStateAlpha->Release();
	    spriteHandler->Release();
        pRenderTargetView->Release();
        pSwapChain->Release();
        pD3DDevice->Release();
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
