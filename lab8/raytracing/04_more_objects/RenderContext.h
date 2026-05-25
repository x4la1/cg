#pragma once

#include "Matrix4.h"
#include "ViewPort.h"

class Scene;
class IColorBuffer;
class Ray;

/*
Класс CRenderContext - контекст визуализации
Выполняет вычисление цвета пикселя в заданных координатах внутри буфера кадра
методом трассировки лучей
Видовой порт задает прямоугольную область в буфере кадра, которая принадлежит контексту рисования
Матрицы моделирования-вида и проецирования аналогично соответствующим матрицам OpenGL
*/
class RenderContext
{
public:
	RenderContext(void);

	/*
	Трассировка лучей по сцене в заданной прямоугольной области экрана
	Работает немного быстрее назависимых вызовов метода CalculatePixelColor
	за счет сокращения числа вычислений для определения направления луча.
	Результат записывается в объект, реализующий интерфейс IColorBuffer
	*/
	void RenderRectangle(Scene const& scene, int left, int top, unsigned width, unsigned height, IColorBuffer & colorBuffer)const;

	/*
	Трассирует путь луча по сцене, проходящего через пиксель с указанными координатами
	Возвращает цвет данного пикселя в формате 0xAARRBBGG
	*/
	std::uint32_t CalculatePixelColor(Scene const& scene, int x, int y)const;

	/*
	Задает параметры видового порта
	*/
	void SetViewPort(ViewPort const& viewPort);

	/*
	Устанавливает матрицу проецирования (перспективного, либо ортографического)
	*/
	void SetProjectionMatrix(Matrix4d const& projectionMatrix);

	/*
	Устанавливает матрицу моделирования-вида, задающую положение и ориентацию камеры в пространстве
	*/
	void SetModelViewMatrix(Matrix4d const& modelViewMatrix);

private:
	/*
	Преобразовывает экранные координаты пикселя в нормализованные экранные координаты
	В нормализованных координатах верхний левый угол видового порта имеет координаты (-1, +1), 
		а правый нижний: (+1, -1)
	*/
	Vector2d GetNormalizedViewportCoord(double x, double y)const;

	/*
	Возвращает матрицу, обратную произведению матриц проецирования и моделирования-вида
	*/
	Matrix4d const& GetInverseModelViewProjectionMatrix()const;

	/*
	Вычисляет координаты точки, имеющей нормализованные экранные координатами scceenX, screenY и глубину depth
	в мировой системе координат.
	Глубине=0 соответствует ближняя плоскость отсечения, задаваемая матрицей проецирования, а глубине=1 - дальняя
	*/
	Vector3d UnProject(double normalizedX, double normalizedY, double depth)const;

	/*
	Вычисляет значение матрицы обратной произведению матриц проецирования и
	моделирования-вида
	*/
	void UpdateInverseModelViewProjectionMatrix();
private:
	// Вычисление направления луча, проходящего в заданных экранных координатах
	Ray CalculateRay(double screenX, double screenY)const;
	// Квантование цвет в 32-битный ARGB-формат (0xAARRGGBB)
	static std::uint32_t QuantizeColor(Vector4f const& color);

	// Матрица проецирования
	Matrix4d	m_projectionMatrix;
	// Матрицам моделирования-вида
	Matrix4d	m_modelViewMatrix;

	// Матрица обратная произведению матриц проецирования и моделирования-вида
	Matrix4d	m_inverseModelViewProjectionMatrix;

	// Видовой порт
	ViewPort	m_viewPort;
};
