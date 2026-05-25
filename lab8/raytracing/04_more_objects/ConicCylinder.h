#pragma once
#include "GeometryObjectWithInitialTransformImpl.h"

/************************************************************************/
/* Геометрический объект "Конический цилиндр"                           */
/* Ось цилиндра совпадает с осью z. Основание находится в плоскости z=0 */
/* "Крышка" находится в плоскости z="высота"                            */
/************************************************************************/
class ConicCylinder :
	public GeometryObjectWithInitialTransformImpl
{
public:
	ConicCylinder(
		double const & height = 1,		// Высота цилиндра (>=0)
		double const& baseRadius = 1,	// Радиус основания (>=0)
		double const& capRadius = 0,	// Радиус крышки (>=0)
		Matrix4d const& tranform = Matrix4d());

	virtual bool Hit(Ray const& ray, Intersection & intersection)const;
private:
	double m_smallRadius;
};
