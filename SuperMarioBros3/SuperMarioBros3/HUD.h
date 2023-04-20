#pragma once
#include "PMeter.h"
#include "CardGift.h"
#include "GameTimer.h"
class CHUD
{
private:
    Point pos;
    CPMeter* pMeter;
    CCardGift* card1;
    CCardGift* card2;
    CCardGift* card3;
    CSprite* hudSprite;
    CGameTimer* timer;
    CFont* scoreText; // Tổng các score đã thu hoạch được
    CFont* coinText; // cứ 100 score = 1 $
    int score, coin;
    bool isEnable;
public:
    CHUD(Point hudPos);
    void LoadSprite();
    void Update();
    void Render();

    void SetPosition(Point pos);
    Point GetPosition();

    void SetPMeter(CPMeter* pMeter);
    CPMeter* GetPMeter();

    void SetCard(int index, std::string state);
    CCardGift* GetCard(int index);

    CGameTimer* GetTimer();

    void AddingScore(int score);
    int GetScore();

    void AddingCoin(int coin);
    int GetCoin();

    void Enable(bool isEnable);
};

