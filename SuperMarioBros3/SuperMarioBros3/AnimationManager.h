#pragma once
#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <map>

#include "Animation.h"

class CAnimationManager;
typedef CAnimationManager* LPAnimationManager;
class CAnimationManager
{
private:
	static LPAnimationManager instance;
	std::map<String, LPAnimation> animations; // Lưu toàn bộ. Ta sẽ k cần animation set như của thầy Dũng vì ta cho mỗi gameObject clone animation những animation thuộc về nó
	// Animation Manager chỉ có chức năng lưu trữ như một database. Mà được lưu trên RAM để tăng tốc độ xử lý
public:
	CAnimationManager();
	bool Init();
	void Clear();
	bool InitAnAnimationSet(String textureName, String filePath);
	bool LoadAnimation(String texName, String filePath);
	void AddAnimation(String id, LPAnimation ani);
	LPAnimation Get(String id);
	LPAnimation Clone(String id); // Copy constructor: Giúp copy ani ra bản sao để mỗi object xài ani riêng. Không bị trùng nhau
	static LPAnimationManager GetInstance();
	~CAnimationManager();
};
#endif

