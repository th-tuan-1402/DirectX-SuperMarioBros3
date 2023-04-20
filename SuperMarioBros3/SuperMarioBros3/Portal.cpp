#include "Portal.h"
#include "Ultis.h"

CPortal::CPortal()
{
}

CPortal::CPortal(Point size)
{
	Init(size);
}

void CPortal::Init(Point size)
{
	this->SetTag(GameObjectTags::Portal);

	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(size);
	box->SetGameObjectAttach(this);
	box->SetName("Portal");
	box->SetDistance(Point(0.0f, 0.0f));
	this->collisionBoxs->push_back(box);
	this->isEnabled = true;

	physiscBody->SetDynamic(false);

	sceneID = "";
	cameraID = -1;
}