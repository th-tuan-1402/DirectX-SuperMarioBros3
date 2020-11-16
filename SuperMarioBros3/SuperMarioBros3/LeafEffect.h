#pragma once
#include "EffectObject.h"
class CLeafEffect :  public CEffectObject
{
private:
	bool isBounce;
	bool isFall;
	DWORD startFallTime;
public:
	CLeafEffect();
	void LoadAnimation();
	virtual void Update(DWORD dt, CCamera* cam);
	void Render(CCamera* cam, int alpha = 255) override;
	void StartEffect();
};

