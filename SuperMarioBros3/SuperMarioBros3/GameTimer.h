#pragma once
#include "Font.h"
class CGameTimer
{
	Point pos;
	CFont* timerFont;
	int currentTime;
	int timerState;
public:
	CGameTimer();
	void Update();
	void Render();
	void SetPosition(Point pos);
	Point GetPosition();
	void StartTimer();
	void ResetTimer(); // Reset to 300
	void ResetToZero(); // Reset to 0
	CFont* GetTimerText();
	void TimerToFont();
	int GetTimerState();
};

