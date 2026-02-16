#pragma once
#include "pch.h"
#include "Circle.h"
#include <utility>

Circle::Circle(uint32_t cx, uint32_t cy, uint32_t radius,
	COLORREF outlineColor,
	COLORREF fillColor,
	uint32_t thickness)
	: m_centerX(cx)
	, m_centerY(cy)
	, m_radius(radius)
	, m_outlineColor(outlineColor)
	, m_fillColor(fillColor)
	, m_thickness(thickness)
{
}

void Circle::Draw(CDC* pDC)
{
	FillCircle(pDC);
	DrawAntiAliased(pDC);
}

void Circle::DrawAntiAliased(CDC* pDC)
{
	int x = 0;
	double y = m_radius;
	double sqaure = m_radius * m_radius;

	while (x < y)
	{
		double idealY = sqrt(sqaure - x * x);
		double intensity1 = y - floor(y);
		double intensity2 = 1.0 - intensity1;

		int y1 = (int)floor(y);
		int y2 = y1 - 1;

		BlendPixel(pDC, m_centerX + x, m_centerY + y1, m_outlineColor, intensity1);
		BlendPixel(pDC, m_centerX + x, m_centerY + y2, m_outlineColor, intensity2);
		BlendPixel(pDC, m_centerX - x, m_centerY + y1, m_outlineColor, intensity1);
		BlendPixel(pDC, m_centerX - x, m_centerY + y2, m_outlineColor, intensity2);
		BlendPixel(pDC, m_centerX + x, m_centerY - y1, m_outlineColor, intensity1);
		BlendPixel(pDC, m_centerX + x, m_centerY - y2, m_outlineColor, intensity2);
		BlendPixel(pDC, m_centerX - x, m_centerY - y1, m_outlineColor, intensity1);
		BlendPixel(pDC, m_centerX - x, m_centerY - y2, m_outlineColor, intensity2);

		BlendPixel(pDC, m_centerX + y1, m_centerY + x, m_outlineColor, intensity1);
		BlendPixel(pDC, m_centerX + y2, m_centerY + x, m_outlineColor, intensity2);
		BlendPixel(pDC, m_centerX - y1, m_centerY + x, m_outlineColor, intensity1);
		BlendPixel(pDC, m_centerX - y2, m_centerY + x, m_outlineColor, intensity2);
		BlendPixel(pDC, m_centerX + y1, m_centerY - x, m_outlineColor, intensity1);
		BlendPixel(pDC, m_centerX + y2, m_centerY - x, m_outlineColor, intensity2);
		BlendPixel(pDC, m_centerX - y1, m_centerY - x, m_outlineColor, intensity1);
		BlendPixel(pDC, m_centerX - y2, m_centerY - x, m_outlineColor, intensity2);
		x++;
		y = idealY;
	}
}

void Circle::DrawHorizontalLine(CDC* pDC, int x1, int x2, int y)
{
	if (x1 > x2)
	{
		std::swap(x1, x2);
	}

	for (int x = x1; x <= x2; ++x)
	{
		pDC->SetPixelV(x, y, m_fillColor);
	}
}

void Circle::DrawLines(CDC* pDC, int x, int y)
{
	DrawHorizontalLine(pDC, m_centerX - x, m_centerX + x, m_centerY + y);
	DrawHorizontalLine(pDC, m_centerX - x, m_centerX + x, m_centerY - y);
	DrawHorizontalLine(pDC, m_centerX - y, m_centerX + y, m_centerY + x);
	DrawHorizontalLine(pDC, m_centerX - y, m_centerX + y, m_centerY - x);
}

void Circle::BlendPixel(CDC* pDC, int x, int y, COLORREF color, double intensity)
{
	CRect rect;



	if (intensity <= 0) return;
	if (intensity >= 1)
	{
		pDC->SetPixelV(x, y, color);
		return;
	}

	COLORREF bgColor = pDC->GetPixel(x, y);

	BYTE r = GetRValue(color);
	BYTE g = GetGValue(color);
	BYTE b = GetBValue(color);

	BYTE bgR = GetRValue(bgColor);
	BYTE bgG = GetGValue(bgColor);
	BYTE bgB = GetBValue(bgColor);

	BYTE newR = (BYTE)(r * intensity + bgR * (1.0 - intensity));
	BYTE newG = (BYTE)(g * intensity + bgG * (1.0 - intensity));
	BYTE newB = (BYTE)(b * intensity + bgB * (1.0 - intensity));

	pDC->SetPixelV(x, y, RGB(newR, newG, newB));
}

void Circle::FillCircle(CDC* pDC)
{
	int fillRadius = m_radius - 1;
	if (fillRadius > 0)
	{
		int x = 0;
		int y = fillRadius;
		int delta = 3 - 2 * fillRadius;
		DrawLines(pDC, x, y);

		while (y >= x)
		{
			if (delta <= 0)
			{
				delta += 4 * x + 6;
			}
			else
			{
				--y;
				delta += 4 * (x - y) + 10;
			}
			++x;

			DrawLines(pDC, x, y);
		}
	}
}