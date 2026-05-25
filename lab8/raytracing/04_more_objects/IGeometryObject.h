#pragma once

#include "IGeometryObject_fwd.h"
#include "Matrix_fwd.h"

class Ray;
class Intersection;

/*
Интерфейс "Геометрический объект"
Предоставляет возможность задания трансформирующей матрицы и возможность нахождения точек пересечения с лучом
*/
class IGeometryObject
{
public:
	/*
	Получение и установка матрицы преобразования объекта
	*/
	virtual Matrix4d const& GetTransform() const = 0;
	virtual void SetTransform(Matrix4d const& transform) = 0;

	// Нахождение точек столкновения луча с объектом
	virtual bool Hit(Ray const& ray, Intersection& intersection) const = 0;

	virtual ~IGeometryObject(void) = default;
};
