#pragma once
#include "IShader.h"
#include "SimpleMaterial.h"

/*
Простой шейдер, выполняющий расчет диффузной составляющей отраженного света
*/
class SimpleDiffuseShader :
	public IShader
{
public:
	SimpleDiffuseShader(SimpleMaterial const& material = SimpleMaterial());

	/*
	В качестве параметра шейдера выступает класс CSimpleMaterial, хранящий в простейшем случае
	диффузный цвет материала поверхности объекта
	*/
	void SetMaterial(SimpleMaterial const& material);

	/*
	Вычисление цвета с объекта
	*/
	virtual Vector4f Shade(ShadeContext const & shadeContext)const;
private:
	SimpleMaterial m_material;
};
