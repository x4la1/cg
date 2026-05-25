#pragma once

#include "Vector4.h"

/*
Простейший материал, для которого задан только лишь диффузный цвет
*/
class SimpleMaterial
{
public:
	SimpleMaterial(void)
	{}

	Vector4f const& GetDiffuseColor()const
	{
		return m_diffuseColor;
	}

	void SetDiffuseColor(Vector4f const& diffuseColor)
	{
		m_diffuseColor = diffuseColor;
	}

private:
	Vector4f m_diffuseColor;
};
