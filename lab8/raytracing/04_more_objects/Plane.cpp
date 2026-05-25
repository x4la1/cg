#include "stdafx.h"
#include "Plane.h"
#include "Intersection.h"
#include "Ray.h"
#include "VectorMath.h"

Plane::Plane(double a, double b, double c, double d, Matrix4d const & transform)
:GeometryObjectImpl(transform)
,m_planeEquation(a, b, c, d)
{
}

bool Plane::Hit(Ray const& ray, Intersection & intersection)const
{
	// Величина, меньше которой модуль скалярного произведения вектора направления луча и 
	// нормали плоскости означает параллельность луча и плоскости
	const double EPSILON = 1e-10;

	// Вместо преобразования плоскости выполняем обратное преобразование луча
	// Результат будет тот же самый
	Ray invRay = Transform(ray, GetInverseTransform());

	// Нормаль к плоскости в системе координат объекта 
	Vector3d normalInObjectSpace = m_planeEquation;

	// Скалярное произведение направления луча и нормали к плоскости
	double normalDotDirection = Dot(invRay.GetDirection(), normalInObjectSpace);

	// Если скалярное произведение близко к нулю, луч параллелен плоскости
	if (fabs(normalDotDirection) < EPSILON)
	{
		// Луч параллелен плоскости - точек пересечения нет
		return false;
	}

	/*
	Находим время пересечения луча с плоскостью, подставляя в уравнение плоскости точку испускания луча
	и деление результата на ранее вычисленное сканярное произведение направления луча и нормали к плоскости
	*/
	double hitTime = -Dot(Vector4d(invRay.GetStart(), 1), m_planeEquation) / normalDotDirection;

	// Нас интересует только пересечение луча с плоскостью в положительный момент времени,
	// поэтому находящуюся "позади" точки испускания луча точку пересечения мы за точку пересечения не считаем
	// Сравнение с величиной EPSILON, а не с 0 нужно для того, чтобы позволить
	// лучам, испущенным с плоскости, оторваться от нее.
	// Это необходимо при обработке вторичных лучей для построения теней и отражений и преломлений
	if (hitTime <= EPSILON)
	{
		return false;
	}

	// Вычисляем точку столкновения с лучом в системе координат сцены в момент столкновения
	Vector3d hitPoint = ray.GetPointAtTime(hitTime);

	// Вычисляем точку столкновеиня с лучом в системе координат объекта
	Vector3d hitPointInObjectSpace = invRay.GetPointAtTime(hitTime);

	// Вычисляем нормаль к плоскости в системе координат объекта
	Vector3d normalInWorldSpace = GetNormalMatrix() * normalInObjectSpace;

	// В список точек пересечения добавляем информацию о найденной точке пересечения
	intersection.AddHit(HitInfo(
		hitTime,	// Когда столкнулись
		*this,		// С кем
		hitPoint, hitPointInObjectSpace,	// Точка соударения луча с поверхностью
		normalInWorldSpace, normalInObjectSpace	// Нормаль к поверхности в точке соударения
		));

	// Точка столкновения есть, возвращаем true
	return true;
}
