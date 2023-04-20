#pragma once
#include "GameObject.h"
class CNodeMap :
    public CGameObject
{
public:
    CNodeMap();
    CNodeMap(Point size);
    RECT DirectionMarioCanMove(Point posMario);
};

