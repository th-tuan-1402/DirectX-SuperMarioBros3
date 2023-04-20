#pragma once
#include "ArrowItemIcon.h"
#include "PIcon.h"
#include <vector>
class CPMeter
{
private:
	// vector bao gồm 6 arrow
	std::vector<CArrowItemIcon*> arrowItemIcons;
	// p button
	CPIcon* pIcon;
	Point pos;
	float pMeterCounting;
	int pMeterState;
	int feverState;
	bool isRaccoonMario;
	bool canfly;
	int previousFeverState, previousPMeterState;
	bool isDecrease;
public:
	CPMeter(Point pos);
	void Update();
	void Render();

	void SetPMeterCounting(float pMeterCounting);
	float GetPMeterCounting();

	void SetFeverState(int feverstate);
	int GetFeverState();

	void SetIsRaccoonMario(bool isRaccoonMario);

	void SetCanFly(bool canfly);
};

