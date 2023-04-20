#pragma once
#include "GameObject.h"
#include "BasePortal.h"
class CPortal : public CGameObject, public CBasePortal
{
public:
    CPortal();
    CPortal(Point size);
    void Init(Point size);
};