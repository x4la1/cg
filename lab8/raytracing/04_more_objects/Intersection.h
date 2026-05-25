#pragma once

#include "Vector3.h"
#include "VectorMath.h"

class IGeometryObject;

/************************************************************************/
/* Класс, хранящий информацию о точке пересечения луча с объектом       */
/************************************************************************/
class HitInfo
{
public:
	// Конструктор по умолчанию - пересечения нет
	HitInfo()
		:m_hitTime(-1)
		,m_pHitObject(nullptr)
	{
	}

	/*
		hitTime - время столкновения луча с объектом
		hitObject -  объект столкновения
		hitPoint - точка столкновения в системе координатах сцены
		hitPointInObjectSpace - точка столкновения в системе координат объекта
	*/
	HitInfo(
		double hitTime, 
		IGeometryObject const& hitObject, 
		Vector3d const& hitPoint, 
		Vector3d const& hitPointInObjectSpace,
		Vector3d const& normal,
		Vector3d const& normalInObjectSpace
		)
		:m_hitPoint(hitPoint)
		,m_hitPointInObjectSpace(hitPointInObjectSpace)
		,m_normal(Normalize(normal))
		,m_normalInObjectSpace(Normalize(normalInObjectSpace))
		,m_hitTime(hitTime)
		,m_pHitObject(&hitObject)
	{
	}

	// Инициализирована ли точка пересечения
	bool IsInitialized()const
	{
		return m_pHitObject != nullptr;
	}

	// Возвращает время столкновения луча с объектом
	double GetHitTime()const
	{
		assert(IsInitialized());
		return m_hitTime;
	}

	// Возвращает координаты точки в мировой системе координат
	Vector3d const& GetHitPoint()const
	{
		assert(IsInitialized());
		return m_hitPoint;
	}
	
	// Возвращает координаты точки в системе координат объекта
	Vector3d const& GetHitPointInObjectSpace()const
	{
		assert(IsInitialized());
		return m_hitPointInObjectSpace;
	}

	// Ссылка на объект, с которым произошло пересечение
	IGeometryObject const & GetHitObject()const
	{
		assert(IsInitialized());
		return *m_pHitObject;
	}

	// Нормаль к поверхности в мировой системе координат
	Vector3d const& GetNormal()const
	{
		return m_normal;
	}

	// Нормаль к поверхности в системе координат объекта
	Vector3d const& GetNormalInObjectSpace()const
	{
		return m_normalInObjectSpace;
	}
private:
	Vector3d m_hitPoint;
	Vector3d m_hitPointInObjectSpace;
	Vector3d m_normal;
	Vector3d m_normalInObjectSpace;
	double m_hitTime;
	IGeometryObject const * m_pHitObject;
};

/*
Класс, хранящий информацию о точках пересечения луча с объектом сцены.
Первые 4 точки пересечения хранятся в кэше (обычный массив)
Остальные - в std::vector
*/
class Intersection
{
	enum
	{
		// Размер кэша точек пересечений
		HIT_CACHE_SIZE = 4,	
	};
public:
	Intersection()
		:m_hitCount(0)
	{
	}

	/*
	Добавляем точку пересечения, возвращаем индекс добавленной точки пересечения
	*/
	size_t AddHit(HitInfo const& hit)
	{
		// Добавляем элемент в кэш, если он не заполнен
		if (m_hitCount < HIT_CACHE_SIZE)
		{
			m_hitCache[m_hitCount] = hit;
		}
		else	// В противном случае - в vector
		{
			m_hits.push_back(hit);
		}

		return m_hitCount++;
	}

	/*
	Возвращает информацию о точке пересечения с указанным индексом
	*/
	HitInfo const& GetHit(size_t index)const
	{
		assert(index < m_hitCount);

		// Возвращаем элемент либо из кэша
		if (index < HIT_CACHE_SIZE)
		{
			return m_hitCache[index];
		}
		else	// либо из vector-а
		{
			return m_hits[index - HIT_CACHE_SIZE];
		}
	}

	size_t GetHitsCount()const
	{
		return m_hitCount;
	}

	// Очистка информации о точках столкновения
	void Clear()
	{
		if (m_hitCount > HIT_CACHE_SIZE)
		{
			m_hits.clear();
		}
		m_hitCount = 0;
	}
private:
	HitInfo m_hitCache[HIT_CACHE_SIZE];
	std::vector<HitInfo> m_hits;
	size_t m_hitCount;
};
