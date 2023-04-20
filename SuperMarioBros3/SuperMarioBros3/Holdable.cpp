#include "Holdable.h"

void CHoldable::Release(bool isFromHigher)
{
	this->holder = NULL;
}

CGameObject* CHoldable::GetHolder()
{
	return holder;
}

void CHoldable::SetHolder(CGameObject* holder)
{
	this->holder = holder;
}

Point CHoldable::GetHoldableNormal()
{
	return normal;
}

void CHoldable::SetHoldableNormal(Point normal)
{
	this->normal = normal;
}

bool CHoldable::IsHolding()
{
	return holder != NULL;
}

