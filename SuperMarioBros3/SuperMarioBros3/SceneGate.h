#pragma once
#include "BasePortal.h"
#include "GameObject.h"
class CSceneGate : public CBasePortal, public CGameObject
{
    RectF pushDirection; // top: 1, right, left, bottom : 0 => đẩy hướng top
public:
    CSceneGate();
    CSceneGate(Point size);
    void LoadAnimation();
    void Render(CCamera* cam, int alpha = 255);
    void Init(Point size);
    RECT DirectionMarioCanMove(Point posMario);

};