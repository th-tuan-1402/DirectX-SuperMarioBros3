#pragma once
#include "GameObject.h"
class CBoomerang :
    public CGameObject
{
private:
    CObjectPool* pool;
    CGameObject* boomerangBrother;
    int attackState;
    Point goalPosition;
    Point startPosition;
public:
    CBoomerang();
    void LoadAnimation() override;
    void Render(CCamera* cam, int alpha) override;
    virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
    void LinkToPool(CObjectPool* pool);
    void SetGoalPosition(Point goal);
    void SetStartPosition(Point start);
    void SetBoomerangBrother(CGameObject* gO);
    bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
    void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
    void SetAttackState(int attackState);
    int GetAttackState();
};

