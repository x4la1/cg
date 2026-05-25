#pragma once

#include "Vector_fwd.h"

class ShadeContext;

/*
Интерфейс "шейдер", выполняющий расчет цвета объекта в заданной точке с использованием
некоторой модели освещения
*/
class IShader
{
public:
	// Выполняет вычисление цвета с использованием указанного контекста закрашиваиня
	virtual Vector4f Shade(ShadeContext const & shadeContext)const = 0;

	virtual ~IShader(void){}
};
