#pragma once
#include "pch.h"
#include "CollisionShape.h"
#include "Common.h"

class CylinderShape : public CollisionShape
{
private:

public:
	CylinderShape(glm::vec3 halfExtents);
};
