#pragma once
#include "pch.h"
#include "CollisionShape.h"

class CapsuleShape : public CollisionShape
{
private:

public:
	CapsuleShape(float radius, float height);
};
