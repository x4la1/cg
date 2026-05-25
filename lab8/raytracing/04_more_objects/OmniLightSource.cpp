#include "stdafx.h"
#include "OmniLightSource.h"
#include "VectorMath.h"

/*
Инициализация параметров источника света (положение и трансформация)
*/
OmniLightSource::OmniLightSource(Vector3d const& position, Matrix4d const& transform)
:LightSourceImpl(transform)
,m_position(position)
,m_constantAttenuation(1)
,m_linearAttenuation(0)
,m_quadraticAttenuation(0)
{
	UpdatePositionInWorldSpace();
}

/*
Установка параметров ослабления света от расстояния до объекта
*/
void OmniLightSource::SetAttenuation(
	double const & constantAttenuation, 
	double const & linearAttenuation, 
	double const & quadraticAttenuation)
{
	m_constantAttenuation = constantAttenuation;
	m_linearAttenuation = linearAttenuation;
	m_quadraticAttenuation = quadraticAttenuation;
}

void OmniLightSource::SetTransform(Matrix4d const& transform)
{
	LightSourceImpl::SetTransform(transform);

	/*
	При установке матрицы трансформации придется вычислить заново положение источника в мировой системе координат
	*/
	UpdatePositionInWorldSpace();
}

void OmniLightSource::UpdatePositionInWorldSpace()
{
	m_positionInWorldSpace = GetTransform() * Vector4d(m_position, 1);
}

Vector3d OmniLightSource::GetDirectionFromPoint(Vector3d const& point)const
{
	/*
	Направление от точки к источнику в мировых координатах
	*/
	return GetPositionInWorldSpace() - point;
}

double OmniLightSource::GetIntensityInDirection(Vector3d const& direction)const
{
	/*
	Вычисляем расстояние (длина вектора направления)
	*/
	double distance = direction.GetLength();

	/*
	Вычисляем интенсивность света с учетом коэффициентов ослабления
	*/
	return 1.0 / (distance * distance * m_quadraticAttenuation + distance * m_linearAttenuation + m_constantAttenuation);
}

Vector3d const& OmniLightSource::GetPositionInWorldSpace()const
{
	return m_positionInWorldSpace;
}

