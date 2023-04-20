#pragma once
#include "GameObject.h"
class CEffectObject: public CGameObject
{
protected:
	Point startPosition;

public:
	CEffectObject();
	void SetStartPosition(Point pos);
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
};

