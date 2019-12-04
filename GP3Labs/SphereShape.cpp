#include "pch.h"
#include "SphereShape.h"

SphereShape::SphereShape(float radius)
{
	m_shape = new btSphereShape(btScalar(radius));
	m_type = CShapeType::SPHERE;
}