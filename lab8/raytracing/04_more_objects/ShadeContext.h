#pragma once

#include "Vector_fwd.h"
class Ray;
class Scene;

/*
Контекст закрашивания, используемый шейдером для вычисления цвета поверхности
Хранит информацию о координатах обрабатываемой точки, нормали и направлении луча, а также ссылку на сцену
(в дальнейшем понадобится для построения теней, а также расчета преломлений/отражений)
*/
class ShadeContext
{
public:
	/*
	Инициализирует контекст закрашивания
	*/
	ShadeContext(
		Scene const& scene,
		Vector3d const& sufracePoint,
		Vector3d const& sufracePointInObjectSpace,
		Vector3d const& surfaceNormal,	// нормаль в мировой системе координат
		Vector3d const& rayDirection	// направление трассируемого луча в мировой системе координат
		) noexcept
		:m_sufracePoint(sufracePoint)
		,m_surfacePointInObjectSpace(sufracePointInObjectSpace)
		,m_surfaceNormal(surfaceNormal)
		,m_rayDirection(rayDirection)
		,m_scene(scene)
	{
	}

	/*
	Возвращает координаты точки в мировой системе координат.
	*/
	Vector3d const& GetSurfacePoint()const noexcept
	{
		return m_sufracePoint;
	}

	/*
	Возвращает координаты точки в системе координат объекта
	*/
	Vector3d const& GetSurfacePointInObjectSpace()const noexcept
	{
		return m_surfacePointInObjectSpace;
	}

	/*
	Возвращает нормаль к поверхности в мировой системе координат
	*/
	Vector3d const& GetSurfaceNormal()const noexcept
	{
		return m_surfaceNormal;
	}

	/*
	Возвращает направление луча, попавшего в данную точку
	*/
	Vector3d const& GetRayDirection()const noexcept
	{
		return m_rayDirection;
	}

	/*
	Возвращает ссылку на сцену
	*/
	Scene const& GetScene()const noexcept
	{
		return m_scene;
	}

private:
	Vector3d const& m_sufracePoint;
	Vector3d const& m_surfacePointInObjectSpace;
	Vector3d const& m_surfaceNormal;
	Vector3d const& m_rayDirection;
	Scene const& m_scene;
};
