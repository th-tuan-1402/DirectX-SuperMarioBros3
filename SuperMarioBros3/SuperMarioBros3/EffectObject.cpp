#include "EffectObject.h"

CEffectObject::CEffectObject()
{
	tag = GameObjectTags::Effect;
}

void CEffectObject::SetStartPosition(Point pos)
{
	this->transform.position = pos;
	this->startPosition = pos;
}

bool CEffectObject::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return false;
}
