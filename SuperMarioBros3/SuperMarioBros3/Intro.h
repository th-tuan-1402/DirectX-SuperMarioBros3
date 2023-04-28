#pragma once
#include "Scene.h"
class CIntro :
    public CScene
{

public:
	CIntro(String path);
	void Load();
	void Unload() override;
};

