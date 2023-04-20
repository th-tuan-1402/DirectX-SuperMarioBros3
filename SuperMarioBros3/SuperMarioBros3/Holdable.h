#pragma once
#include "GameObject.h"
class CHoldable
{
protected:
	CGameObject* holder;
	Point normal;
public:
	virtual void SetHoldablePosition(Point pos) = 0;
	virtual void Release(bool isFromHigher);
	virtual Point GetHoldableCollisionBox() = 0;

	virtual CGameObject* GetHolder();
	virtual void SetHolder(CGameObject* holder);
	virtual Point GetHoldableNormal();
	virtual void SetHoldableNormal(Point normal);
	virtual bool IsHolding();
};

