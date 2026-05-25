#pragma once
#include "LightSourceImpl.h"
#include "Vector4.h"

/*
Класс "Точечный источник света", характеризующийся позицией в пространстве.
Для данного типа источника света можно задать коэффициенты ослабления света в зависимости
от расстояния до объекта
*/
class OmniLightSource : public LightSourceImpl
{
public:
	OmniLightSource(
		Vector3d const& position = Vector3d(),
		Matrix4d const& transform = Matrix4d());

	/*
	Перегруженный метод установки матрицы трансформации
	*/
	virtual void SetTransform(Matrix4d const& transform);

	/*
	Вычисление направления из точки пространства к источнику света
	*/
	virtual Vector3d GetDirectionFromPoint(Vector3d const& point) const;

	/*
	Вычисление интенсивности света, излучаемой в заданном направлении
	*/
	virtual double GetIntensityInDirection(Vector3d const& direction) const;

	/*
	Установка коэффициентов ослабления излучаемой интенсивности в зависимости от расстояния
	*/
	void SetAttenuation(
		double const& constantAttenuation,
		double const& linearAttenuation,
		double const& quadraticAttenuation);

private:
	// Получение позиции источника света в мировых координатах
	Vector3d const& GetPositionInWorldSpace() const;

	//	Обновление позиции источника света в мировых координатах в зависимости от матрицы трансформации
	void UpdatePositionInWorldSpace();

private:
	// Начальная позиция источника света
	Vector3d m_position;

	/*
	Вычисленная и закешированная позиция источника света в мировых координатах
	*/
	Vector3d m_positionInWorldSpace;

	// Коэффициенты ослабления света в зависимосит от расстояния
	double m_constantAttenuation;
	double m_linearAttenuation;
	double m_quadraticAttenuation;
};

using COmniLightPtr = std::shared_ptr<OmniLightSource>;