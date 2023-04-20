#pragma once
#include "GameObject.h"
#include "PhysicsBody.h"
#include "RectF.h"

// Vật mà Body sẽ dùng để nhận ra sự va chạm. Găn liền với mỗi game object

class CGameObject;
typedef CGameObject* LPGameObject;

class CPhysicsBody;
typedef CPhysicsBody* LPPhysicsBody;

class CCamera;

struct CollisionEvent;

class CCollisionBox
{
protected:
	LPGameObject gameObject;
	Point sizeBox;
	Point localPosition; 
	Point distance;  //dx = vx*dt 
	std::string name;
	int id;
	bool isEnable;
public:
	CCollisionBox();
	~CCollisionBox();
	void Render(CCamera* camera, int distance);
	virtual void CollisionHandle(DWORD dt, std::vector<CollisionEvent*>& collisions, LPPhysicsBody phyBody, Point vel, int mintx, int minty, float nx, float ny);

	virtual void SetGameObjectAttach(LPGameObject gO);
	LPGameObject GetGameObjectAttach();
	
	void SetSizeBox(Point size);
	Point GetSizeBox();

	void SetPosition(Point pos); // Local Position
	Point GetPosition();

	void SetDistance(Point d);
	Point GetDistance();

	Point GetWorldPosition();

	bool IsEnabled();
	void SetEnable(bool isEnable);

	RectF GetBoundingBox();

	void SetName(std::string n);
	std::string GetName();

	void SetId(int i);
	int GetID();
};
typedef CCollisionBox* LPCollisionBox;