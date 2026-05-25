#include "stdafx.h"
#include "SimpleDiffuseShader.h"
#include "Vector4.h"
#include "Scene.h"
#include "ShadeContext.h"
#include "ILightSource.h"
#include "VectorMath.h"


SimpleDiffuseShader::SimpleDiffuseShader(SimpleMaterial const& material)
:m_material(material)
{
}

/*
Запоминаем параметры материала, связанного с шейдером
*/
void SimpleDiffuseShader::SetMaterial(SimpleMaterial const& material)
{
	m_material = material;
}

Vector4f SimpleDiffuseShader::Shade(ShadeContext const & shadeContext)const
{
	/*
	Получаем сцену из контекста закрашивания для того, чтобы вычислить вклад
	каждого из источников света в освещенность обрабатываемой точки
	*/
	Scene const& scene = shadeContext.GetScene();

	// Результирующий цвет
	Vector4f shadedColor;

	const size_t numLights = scene.GetLightsCount();

	// Пробегаемся по всем источникам света в сцене
	for (size_t i = 0; i < numLights; ++i)
	{
		// Получаем источник света
		ILightSource const& light = scene.GetLight(i);

		// Вычисляем вектор направления на источник света из текущей точке
		Vector3d lightDirection = light.GetDirectionFromPoint(shadeContext.GetSurfacePoint());

		// Вычисляем интенсивность света в направлении от источника к текущей точке
		double lightIntensity = light.GetIntensityInDirection(-lightDirection);

		// Получаем нормаль к поверхности в обрабатываемой точке
		Vector3d const& n = shadeContext.GetSurfaceNormal();

		// Вычисляем скалярное произведение нормали и орт-вектора направления на источник света
		double nDotL = Max(Dot(n, Normalize(lightDirection)), 0.0);
		
		// Вычисляем диффузный цвет точки
		Vector4f diffuseColor = static_cast<float>(nDotL * lightIntensity) * light.GetDiffuseIntensity() * m_material.GetDiffuseColor();

		// К результирующему цвету прибавляется вычисленный диффузный цвет
		shadedColor += diffuseColor;
	}	// Проделываем данные действия для других источников света

	// Возвращаем результирующий цвет точки
	return shadedColor;
}