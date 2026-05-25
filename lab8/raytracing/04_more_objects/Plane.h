#pragma once
#include "GeometryObjectImpl.h"
#include "Vector4.h"
#include "Matrix_fwd.h"

/*
Геометрический объект "бесконечная плоскость"
*/
class Plane : public GeometryObjectImpl
{
public:
	/*
	Плоскость задается коэффициентами уравления плоскости ax+by+cz+d=0,
	а также матрицей начального преобразования плоскости
	*/
	Plane(double a, double b, double c, double d, Matrix4d const & transform = Matrix4d());

	/*
	Нахождение точки пересечения луча с плоскостью
	*/
	virtual bool Hit(Ray const& ray, Intersection & intersection)const;
private:
	// Четырехмерный вектор, хранящий коэффициенты уравнения плоскости
	Vector4d m_planeEquation;
};
