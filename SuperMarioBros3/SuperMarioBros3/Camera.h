#pragma once
#include "Type.h"
#include "D3DType.h"
#include "GameObject.h"
#include "RectF.h"

// là 1 object đặc biệt, có thể có nhiều camera (onGround, underGround)
class CGameObject;
typedef CGameObject* LPGameObject;

struct CameraPropertieSet;

class CCamera 
{
protected:
	DWORD dt; // deltatime
	float vx;
	Point posCam;
	float widthCam;
	float heightCam;

	LPGameObject gameObject; // lưu cái gameobject mà camera follow: Mario
	bool isDisablePosX;
	bool isDisablePosY;
	bool isAutoX;
	RectF currentBoundary;
	std::unordered_map<int, CameraPropertieSet> cameraPropertieSets;
public:
	CCamera();
	CCamera(int wid, int hei);
	~CCamera();

	virtual void Update();
	virtual void Render();

	Point Transform(Point posWorld);
	Point TransformCamToWorld(Point posInCam);
	bool CheckObjectInCamera(LPGameObject gO); //Kiểm tra xem obj truyền vào có nằm trong camera không để ta xử lý update / renderm
	bool CheckRectInCamera(RECT rect);

	int GetSpeedXCam();
	void SetSpeedXCam(float v);

	Point GetPositionCam();
	virtual void SetPositionCam(Point pos);

	float GetWidthCam();
	void SetWidthCam(float w);

	float GetHeightCam();
	void SetHeightCam(float h);

	RectF GetCurrentBoundary();
	void SetCurrentBoundary(RectF bound);

	CameraPropertieSet GetCameraProperties(int id);
	void AddCameraProperties(int id, Point pos, RectF boundary, bool isDisableX, bool isDiableY);
	void AddCameraProperties(int id, CameraPropertieSet camProps);

	LPGameObject GetGameObject();
	void SetGameObject(LPGameObject gO);

	bool GetDisablePosX();
	void SetDisablePosX(bool isDisableX);

	bool GetDisablePosY();
	void SetDisablePosY(bool isDisableY);

	bool GetAutoX();
	void SetAutoX(bool isAutoX);
};

struct CameraPropertieSet
{
	Point camPosition; // tọa độ trái trên
	RectF boundarySet;
	bool disableX;
	bool disableY;
	static CameraPropertieSet Empty()
	{
		Point pos(-1, -1);
		RectF rect;
		rect.left = -1;
		rect.right = -1;
		rect.top = -1;
		rect.bottom = -1;
		return CameraPropertieSet{ pos, rect };
	}
	static bool IsEmpty(CameraPropertieSet x)
	{
		if (x.boundarySet.bottom == -1 && x.boundarySet.left == -1 && x.boundarySet.top == -1 && x.boundarySet.right == -1)
			if (x.camPosition.x == -1 && x.camPosition.y == -1)
				return true;
		return false;
	}
};