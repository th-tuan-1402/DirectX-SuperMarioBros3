#pragma once
#include "Enemy.h"
class CBoomerangBrother :
    public CEnemy
{
private:
    CObjectPool boomerangs;
    int moveState;
    bool canAttack;
    bool canThrowSecondBoomerang;
    int idleTimer;
    CGameObject* onHoldObject;
    bool isHoldBoomerang;
public:
    CBoomerangBrother();
    void LoadAnimation() override;
    void Render(CCamera* cam, int alpha) override;
    virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
    CObjectPool GetObjectPool();
    void OnAttack(Point normal);
    void OnMovingForward(Point normal);
    void OnMovingBackwards(Point normal);
    void OnHoldBoomerang(Point normal);
    void OnDie() override;
};

