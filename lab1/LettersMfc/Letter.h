#pragma once
#include <cmath>
#include "framework.h"
const double JUMP_AMPLITUDE = 80.0;
const double JUMP_PERIOD = 1.0;

struct GdiResources
{
	CPen whitePen;
	CBrush whiteBrush;
	CPen bluePen;
	CBrush blueBrush;
	CPen redPen;
	CBrush redBrush;
};

class CDC;

class Letter
{
public:
	Letter(int size, double phaseOffset)
		:m_size(size), m_phaseOffset(phaseOffset)
	{
	}

	virtual void Draw(CDC* pDC, GdiResources& gdiResources, double animationTime, int centerX, int centerY) = 0;

	double CalculateBounceOffset(double time) const
	{
		double adjustedTime = time + m_phaseOffset;
		double t = fmod(adjustedTime, JUMP_PERIOD);
		double normalizedTime = t / JUMP_PERIOD;
		return JUMP_AMPLITUDE * (1.0 - 4 * pow(normalizedTime - 0.5, 2));
	}

	virtual ~Letter() {}
protected:
	int m_size;
	double m_phaseOffset;
};

class LetterL : public Letter
{
public:
	LetterL(int size, double phaseOffset)
		:Letter(size, phaseOffset)
	{
	}

	void Draw(CDC* pDC, GdiResources& gdiResources, double animationTime, int centerX, int centerY) override
	{
		double offset = CalculateBounceOffset(animationTime);
		double currentY = centerY - offset;
		double halfSize = m_size / 2;

		CPen* oldPen = pDC->SelectObject(&gdiResources.whitePen);
		CBrush* oldBrush = pDC->SelectObject(&gdiResources.whiteBrush);

		POINT letterPoints[] = {
		{ centerX, static_cast<LONG>(currentY - halfSize) },
		{ static_cast<LONG>(centerX + halfSize), static_cast<LONG>(currentY + halfSize) },
		{ static_cast<LONG>(centerX + (halfSize * 0.6)), static_cast<LONG>(currentY + halfSize) },
		{ centerX, static_cast<LONG>(currentY - (halfSize * 0.2)) },
		{ static_cast<LONG>(centerX - (halfSize * 0.6)), static_cast<LONG>(currentY + halfSize) },
		{ static_cast<LONG>(centerX - halfSize), static_cast<LONG>(currentY + halfSize) },
		};

		pDC->Polygon(letterPoints, 6);

		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}
};

class LetterD : public Letter
{
public:
	LetterD(int size, double phaseOffset)
		:Letter(size, phaseOffset)
	{
	}

	void Draw(CDC* pDC, GdiResources& gdiResources, double animationTime, int centerX, int centerY) override
	{
		double offset = CalculateBounceOffset(animationTime);
		int currentY = centerY - offset;
		double halfSize = m_size / 2;

		CPen* oldPen = pDC->SelectObject(&gdiResources.bluePen);
		CBrush* oldBrush = pDC->SelectObject(&gdiResources.blueBrush);

		pDC->Rectangle(
			static_cast<int>(centerX - halfSize),
			static_cast<int>(currentY + (halfSize * 0.4)),
			static_cast<int>(centerX + halfSize),
			static_cast<int>(currentY + (halfSize * 0.8)));

		pDC->Rectangle(
			static_cast<int>(centerX - halfSize),
			static_cast<int>(currentY + (halfSize * 0.8)),
			static_cast<int>(centerX - (halfSize * 0.6)),
			static_cast<int>(currentY + halfSize));

		pDC->Rectangle(
			static_cast<int>(centerX + (halfSize * 0.6)),
			static_cast<int>(currentY + (halfSize * 0.8)),
			static_cast<int>(centerX + halfSize),
			static_cast<int>(currentY + halfSize));

		pDC->Rectangle(

			static_cast<int>(centerX - (halfSize * 0.6)),
			static_cast<int>(currentY - halfSize),
			static_cast<int>(centerX + (halfSize * 0.6)),
			static_cast<int>(currentY - (halfSize * 0.6)));

		pDC->Rectangle(
			static_cast<int>(centerX - (halfSize * 0.6)),
			static_cast<int>(currentY - (halfSize * 0.6)),
			static_cast<int>(centerX - (halfSize * 0.2)),
			static_cast<int>(currentY + (halfSize * 0.4)));

		pDC->Rectangle(
			static_cast<int>(centerX + (halfSize * 0.2)),
			static_cast<int>(currentY - (halfSize * 0.6)),
			static_cast<int>(centerX + (halfSize * 0.6)),
			static_cast<int>(currentY + (halfSize * 0.4)));


		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}
};

class LetterA : public Letter
{
public:
	LetterA(int size, double phaseOffset)
		:Letter(size, phaseOffset)
	{
	}

	void Draw(CDC* pDC, GdiResources& gdiResources, double animationTime, int centerX, int centerY) override
	{
		double offset = CalculateBounceOffset(animationTime);
		int currentY = centerY - offset;
		double halfSize = m_size / 2;

		CPen* oldPen = pDC->SelectObject(&gdiResources.redPen);
		CBrush* oldBrush = pDC->SelectObject(&gdiResources.redBrush);

		POINT letterPoints[] = {
		{ centerX, static_cast<LONG>(currentY - halfSize) },
		{ static_cast<LONG>(centerX + halfSize), static_cast<LONG>(currentY + halfSize) },
		{ static_cast<LONG>(centerX + (halfSize * 0.6)), static_cast<LONG>(currentY + halfSize) },
		{ centerX, currentY - (halfSize * 0.2) },
		{ static_cast<LONG>(centerX - (halfSize * 0.6)), static_cast<LONG>(currentY + halfSize) },
		{ static_cast<LONG>(centerX - halfSize), static_cast<LONG>(currentY + halfSize) },
		};

		pDC->Polygon(letterPoints, 6);
		pDC->Rectangle(
			static_cast<int>(centerX - (halfSize * 0.4)),
			static_cast<int>(currentY + (halfSize * 0.4)),
			static_cast<int>(centerX + (halfSize * 0.4)),
			static_cast<int>(currentY + (halfSize * 0.6)));

		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}
};
