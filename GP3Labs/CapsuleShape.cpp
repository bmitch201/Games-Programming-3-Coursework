#include "pch.h"
#include "CapsuleShape.h"

CapsuleShape::CapsuleShape(float radius, float height)
{
	m_shape = new btCapsuleShape(btScalar(radius), btScalar(height));
	m_type = CShapeType::CAPSULE;
}