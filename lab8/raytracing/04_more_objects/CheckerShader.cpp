#include "stdafx.h"
#include "CheckerShader.h"
#include "VectorMath.h"
#include "ShadeContext.h"

CheckerShader::CheckerShader(Matrix4d const& textureTransform)
:m_textureTransform(textureTransform)
{
}

void CheckerShader::SetTextureTransform(Matrix4d const& textureTransform)
{
	m_textureTransform = textureTransform;
}

Vector4f CheckerShader::Shade(ShadeContext const & shadeContext)const
{
	/*
	Шейдер шахматной доски подвергает точку, заданную в системе координат объекта,
	преобразованию, заданному матрицей трансформации.
	У полученной точки определяется принадлежность к черной или белой клетке трехмерного шахматного пространства
	*/

	// Представление точки в виде 4-мерного вектора
	Vector4d pt(shadeContext.GetSurfacePointInObjectSpace(), 1.0);
	// Трансформируем координаты матрицей трансформации текстурных координат
	Vector3d transformedPoint = (m_textureTransform * pt).Project();
	/*
	Вычисляем дробную часть координат точки в систем координат объекта
	*/
	Vector3d fract = Fract(transformedPoint);
	/*
	Координаты, превышающие 0.5, будут иметь значение 1, а не превышающие - 0
	*/
	Vector3d s = Step(0.5, fract);

	/*
	Применяем операцию XOR для определения принадлежности точки либо к черному, либо к белому кубу
	*/
	if (int(s.x) ^ int(s.y) ^ int(s.z))
	{
		return Vector4f(0.9f, 0.9f, 0.9f, 1);
	}
	else
	{
		return Vector4f(0.1f, 0.1f, 0.1f, 1);
	}

}