#include "pch.h"
#include "ConeShape.h"
ConeShape::ConeShape(float radius, float height){	m_shape = new btConeShape(btScalar (radius), btScalar (height));	m_type = CShapeType::CONE;}