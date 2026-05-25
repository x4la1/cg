#pragma once
#include "IShader.h"
#include "Matrix4.h"
/*
Шейдер шахматной доски
*/
class CheckerShader :
	public IShader
{
public:
	// Инициализация шейдера матрицей преобразования текстурных координат
	CheckerShader(Matrix4d const& textureTransform = Matrix4d());

	void SetTextureTransform(Matrix4d const& textureTransform);

	virtual Vector4f Shade(ShadeContext const & shadeContext)const;
private:
	Matrix4d m_textureTransform;
};
