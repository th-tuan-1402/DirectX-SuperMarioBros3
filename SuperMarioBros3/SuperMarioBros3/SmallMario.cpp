#include "SmallMario.h"
#include "MarioConst.h"
#include "MarioControllerConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
CSmallMario::CSmallMario()
{
	CSmallMario::Init();
	CMario::InitProperties();
}
void CSmallMario::Init()
{
	SetTag(GameObjectTags::SmallMario);
	LoadAnimation();
	//DebugOut(L"Init Small Mario \n");
	CCollisionBox* collisionBox = new CCollisionBox(); 
	//collisionBox->SetSizeBox(SUPER_MARIO_BBOX);
	collisionBox->SetSizeBox(D3DXVECTOR2(12 * 3, 15 * 3));
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario"); // SmallMario
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);
	//DebugOut(L"Collision Box: size (w,h): (%f, %f)", collisionBox->GetSizeBox().x, collisionBox->GetSizeBox().y);
	canCrouch = false;
}

void CSmallMario::LoadAnimation()
{
	DebugOut(L"Load animation Small Mario \n");
	//DebugOut(L"Collision Box: size (w,h): (%f, %f)", collisionBox->GetSizeBox().x, collisionBox->GetSizeBox().y);

	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-small-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-small-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-small-mario-run"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-small-mario-jump"));

	CSmallMario::SetState(MARIO_STATE_IDLE);
}
