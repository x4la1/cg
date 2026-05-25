#pragma once

#include "GeometryObjectWithInitialTransformImpl.h"

class Torus : public GeometryObjectWithInitialTransformImpl
{
public:
	Torus(
		double majorRadius = 1,
		double tubeRadius = 0.25,
		Vector3d const& center = Vector3d(),
		Matrix4d const& transform = Matrix4d());

	virtual bool Hit(Ray const& ray, Intersection& intersection) const;

private:
	double m_majorRadius;
	double m_tubeRadius;
};