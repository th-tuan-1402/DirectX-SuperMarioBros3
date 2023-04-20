#pragma once

#ifndef ANIMATINO_H
#define ANIMATION_H

#include <string>
#include <vector>
#include "AnimationFrame.h"
#include "Sprite.h"
#include "Transform.h"
#include "GameObject.h"
class CGameObject;
typedef CGameObject* LPGameObject;

class CAnimation;
typedef CAnimation* LPAnimation;
class CAnimation
{
private:
	std::string id;
	DWORD defaultFrameTime, lastFrameTime;
	int currentFrame;
	bool isLoop, isPlaying;
	Transform transform;
	Point relativePosition;
	std::vector<LPAnimationFrame> animFrames;
	CGameObject* gameObject; // Lưu con trỏ gameobject mà animation thuộc về để gọi animation end

public: 
	CAnimation(std::string aniID, DWORD defaultTime = 100);
	CAnimation(const CAnimation& obj);
	void Add(LPSprite sprite, DWORD frameTime = 0);
	void Render(Point position, int alpha = 254);

	void SetPosition(Point p) { this->transform.position = p; } // Relative Position. Sẽ có lúc tọa độ thực giữ nguyên nhưng ta thay đổi tọa đọ vẽ lên màn hình
	void SetScale(Point s) { this->transform.scale = s; }
	void SetRotation(float r) { this->transform.rotationAngle = r; }
	void SetLoopAnimation(bool isLoop) { this->isLoop = isLoop; }
	void SetRelativePosition(Point rP) { this->relativePosition = rP; }
	void SetPlay(bool isPause);
	Point GetPosition() { return transform.position; }
	Point GetScale() { return transform.scale; }
	float GetRotation() { return transform.rotationAngle; }
	bool GetLoopAnimation() { return isLoop; }
	LPAnimationFrame GetAnimFrame();
	Point GetRelativePosition() { return relativePosition; }

	void SetGameObject(LPGameObject gO) { gameObject = gO; }
	LPGameObject GetGameObject() { return gameObject; }
	void ResetAnimation();
	~CAnimation();
};

#endif
