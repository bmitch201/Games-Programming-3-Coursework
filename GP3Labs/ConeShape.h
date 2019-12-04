#pragma once
#include "pch.h"
#include "CollisionShape.h"

class ConeShape : public CollisionShape
{
private: 

public:
	ConeShape(float radius, float height);
};
