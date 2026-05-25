#pragma once
#include "GeometryObjectWithInitialTransformImpl.h"

/************************************************************************/
/* Геометрический объект с начальной трансформацией "Сфера"             */
/************************************************************************/
class Sphere :
	public GeometryObjectWithInitialTransformImpl
{
public:
	/*
	Инициализация сферы заданного радиуса и положения в пространстве
	*/
	Sphere(
		double radius = 1, 
		Vector3d const& center = Vector3d(), 
		Matrix4d const& transform = Matrix4d());

	/*
	Пересечение луча со сферой
	*/
	virtual bool Hit(Ray const& ray, Intersection & intersection)const;
private:
};
