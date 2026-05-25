#pragma once

#include "Vector3.h"

/*
Класс "Луч", характеризующийся точкой испускания и направлением
В момент времени t=0 луч находится в стартовой точке.
За промежуток времени t=1 луч проходит расстояние direction
*/
class Ray
{
public:
	Ray(Vector3d const& start, Vector3d const& direction) noexcept
		:m_start(start), m_direction(direction)
	{
	}

	// Возвращает точку на луче в указанный момент времени
	Vector3d GetPointAtTime(double time)const noexcept
	{
		return m_start + m_direction * time;
	}

	/*
	Начальная точка луча
	*/
	Vector3d const& GetStart()const noexcept
	{
		return m_start;
	}

	/*
	Направление луча
	*/
	Vector3d const& GetDirection()const noexcept
	{
		return m_direction;
	}

private:
	Vector3d m_start;
	Vector3d m_direction;
};

#include "VectorMath.h"
#include "Matrix4.h"

/*
Трансформация луча с использованием заданной матрицы
*/
inline Ray Transform(Ray const& ray, Matrix4d const& matrix) noexcept
{
	Vector3d start = (matrix * Vector4d(ray.GetStart(), 1)).Project();
	Vector3d direction = (matrix * Vector4d(ray.GetDirection(), 0));
	return Ray(start, direction);
}