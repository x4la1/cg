#pragma once
#include "ILightSource.h"
#include "Matrix4.h"
#include "Vector4.h"

/*
Реализация методов, общих для различных типов источников света
*/
class LightSourceImpl :
	public ILightSource
{
public:
	/*
	Получение матрицы трансформации источника света
	*/
	Matrix4d const& GetTransform()const
	{
		return m_transform;
	}

	/*
	Установка матрицы трансформации источника света
	*/
	void SetTransform(Matrix4d const& transform)
	{
		m_transform = transform;
	}

	/*
	Установка интенсивности диффузной составляющей света
	*/
	virtual void SetDiffuseIntensity(Vector4f const& diffuseIntensity)
	{
		m_diffuseIntensity = diffuseIntensity;
	}

	/*
	Получение интенсивности диффузной составляющей источника света
	*/
	virtual Vector4f const& GetDiffuseIntensity()const
	{
		return m_diffuseIntensity;
	}

protected:
	LightSourceImpl(Matrix4d const& transform = Matrix4d())
		:m_transform(transform)
	{
	}

private:
	Vector4f m_diffuseIntensity;
	Matrix4d m_transform;

};
