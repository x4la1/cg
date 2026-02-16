#pragma once
#include "pch.h"
#include <cstdint>

class Circle
{
public:
	Circle(uint32_t cx = 100, uint32_t cy = 100, uint32_t radius = 50,
		COLORREF outlineColor = RGB(255, 255, 255),
		COLORREF fillColor = RGB(255, 0, 0),
		uint32_t thickness = 1
	);

	virtual ~Circle() {}

	void Draw(CDC* pDC);

private:
	void DrawAntiAliased(CDC* pDC);

	void DrawHorizontalLine(CDC* pDC, int x1, int x2, int y);
	void DrawLines(CDC* pDC, int x, int y);
	void BlendPixel(CDC* pDC, int x, int y, COLORREF color, double intensity);
	void FillCircle(CDC* pDC);

	uint32_t m_centerX;
	uint32_t m_centerY;
	uint32_t m_radius;

	COLORREF m_outlineColor;
	COLORREF m_fillColor;
	uint32_t m_thickness;
};