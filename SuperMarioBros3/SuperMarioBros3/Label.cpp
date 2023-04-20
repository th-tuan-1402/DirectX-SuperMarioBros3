#include "Label.h"
#include "Ultis.h"

CLabel::CLabel(Point size)
{
	this->SetTag(GameObjectTags::Label);
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(size);
	box->SetGameObjectAttach(this);
	box->SetName("Label");
	box->SetDistance(Point(0.0f, 0.0f));
	this->collisionBoxs->push_back(box);
	this->isEnabled = true;

	physiscBody->SetTrigger(true);
	physiscBody->SetDynamic(false);
}

void CLabel::SetPushDirection(RectF pushDirect)
{
	pushDirection = pushDirect;
}

RectF CLabel::GetPushDirection()
{
	return pushDirection;
}

bool CLabel::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return false;
}
