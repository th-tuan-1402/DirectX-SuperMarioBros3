#include "UICamera.h"
#include "Game.h"
#include "TextureManager.h"
#include "Const.h"
#include "UICameraConst.h"

CUICamera::CUICamera()
{
}

CUICamera::CUICamera(int wid, int hei, D3DXVECTOR2 hudPos)
{
	hud = new CHUD(hudPos);
    this->widthCam = wid;
    this->heightCam = hei;
    disableBlackTexture = false;
}

void CUICamera::Update()
{
    hud->Update();
}

void CUICamera::Render()
{
    if (disableBlackTexture == false)
    {
        auto tex = CTextureManager::GetInstance()->GetTexture(TEXTURE_BLACK);
        D3DXVECTOR2 posInCam;
        posInCam.x = trunc(hud->GetPosition().x - this->posCam.x);
        posInCam.y = trunc(hud->GetPosition().y - this->posCam.y) + BLACK_RECTANGLE_HEIGHT;
        float surfaceWidth = surfaceRect.right - surfaceRect.left;
        float surfaceHeight = surfaceRect.bottom - surfaceRect.top;
        CGame::GetInstance()->Draw(posInCam, D3DXVECTOR2(surfaceWidth * 0.5f, surfaceHeight * 0.5f), tex, surfaceRect, D3DCOLOR_XRGB(0, 0, 0));
    }
	if (hud != NULL)
    {
       hud->Render();
    }
    if (texts.size() > 0)
    {
        for (auto text : texts)
            text->Render();
    }
}

CHUD* CUICamera::GetHUD()
{
    return hud;
}

void CUICamera::SetHUD(CHUD* hud)
{
    this->hud = hud;
}

void CUICamera::AddText(CFont* text)
{
    if (text == NULL) return;
    texts.push_back(text);
}

void CUICamera::SetPositionCam(D3DXVECTOR2 pos)
{
    CCamera::SetPositionCam(pos);
    surfaceRect.left = 0;
    surfaceRect.top = 0;
    surfaceRect.right = surfaceRect.left + widthCam + BLACK_RECTANGLE_WIDTH;
    surfaceRect.bottom = surfaceRect.top + heightCam;
}

void CUICamera::SetDisableBlackTexture(bool disT)
{
    this->disableBlackTexture = disT;
}

CUICamera::~CUICamera()
{

}
