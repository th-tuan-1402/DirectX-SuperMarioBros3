#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Type.h"

struct Transform
{
	Point position;
	Point scale;
	float rotationAngle; // nhận vào là độ. Khi vào draw phải chuyển sang rad
	
	Transform CreateTransformation(Point position, Point scale, float rotationAngle)
	{
		Transform result;
		result.position = position;
		result.scale = scale;
		result.rotationAngle = rotationAngle;
		return result;
	}
};


#endif