﻿#include "QuestionBlock.h"
#include "AnimationManager.h"
#include "QuestionBlockConst.h"
#include "CoinEffect.h"
#include "SceneManager.h"
#include "Ultis.h"
#include "Game.h"
#include "LeafEffect.h"
#include "MushroomEffect.h"
CQuestionBlock::CQuestionBlock()
{
	CBlock::CBlock();
	SetTag(GameObjectTags::QuestionBlock);
	LoadAnimation();
	SetState(QB_STATE_SEALED);
	isEnabled = true;
	bounceState = 0;
	startBounceTime = 0;
	bounceDelta = 0.0f;
}

void CQuestionBlock::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(QB_STATE_SEALED, animations->Get("ani-question-block"));
	AddAnimation(QB_STATE_EMPTY, animations->Get("ani-empty-block"));
}

void CQuestionBlock::Init()
{

}


void CQuestionBlock::SetItemInfo(ItemInfo info)
{
	this->itemInfo = info;
}

void CQuestionBlock::Bounce()
{
	if (bounceState == 0)
	{
		startBounceTime = GetTickCount64();
		bounceState = 1;
		if (itemInfo.quantity >= 0)
		{
			switch (itemInfo.tag)
			{
				case ItemTag::Coin:
				{
					CCoinEffect* coinObtainedFX = new CCoinEffect();
					coinObtainedFX->SetStartPosition(transform.position);
					auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
					activeScene->AddObject(coinObtainedFX);
					break;
				}
				case ItemTag::SuperLeaf:
				{
					CLeafEffect* leafObtainedFX = new CLeafEffect();
					leafObtainedFX->SetStartPosition(transform.position);
					leafObtainedFX->StartEffect();
					auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
					activeScene->AddObject(leafObtainedFX);
					break;
				}
				case ItemTag::SuperMushroom:
				{
					CMushroomEffect* mushroomObtainedFX = new CMushroomEffect();
					mushroomObtainedFX->SetStartPosition(transform.position);
					mushroomObtainedFX->StartEffect();
					auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
					activeScene->AddObject(mushroomObtainedFX);
					break;
				}
			}
			itemInfo.quantity--;
			if (itemInfo.quantity <= 0)
			{
				if (itemInfo.quantity < 0)
					itemInfo.quantity = 0;
				if (currentState != QB_STATE_EMPTY)
					SetState(QB_STATE_EMPTY);
			}
		}
	}
}

void CQuestionBlock::Update(DWORD dt, CCamera* cam)
{
	if (bounceState == 2)
	{
		bounceDelta = BOUNCE_VEL;

		if (GetTickCount64() - startBounceTime > BOUNCE_TIME && startBounceTime != 0)
		{
			bounceDelta = 0.0f;
			startBounceTime = 0;
			bounceState = 0;
			if (itemInfo.quantity <= 0)
				bounceState = -1;
		}
	}
	if (bounceState == 1)
	{
		bounceDelta = -BOUNCE_VEL;

		if (GetTickCount64() - startBounceTime > BOUNCE_TIME && startBounceTime != 0)
		{
			bounceState = 2;
			startBounceTime = GetTickCount64();
		}
	}
	
}

void CQuestionBlock::Render(CCamera* cam, int alpha)
{
	relativePositionOnScreen.y += bounceDelta;
	CGameObject::Render(cam, alpha);
}