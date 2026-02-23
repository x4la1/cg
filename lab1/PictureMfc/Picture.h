#pragma once
#include "pch.h"

struct GdiResources
{
	CPen whitePen;
	CBrush whiteBrush;
	CPen bluePen;
	CBrush blueBrush;
	CPen redPen;
	CBrush redBrush;
};
class SimplePicture
{
public:
	SimplePicture(int x = 0, int y = 0) : m_leftTop(x, y) {}
	virtual ~SimplePicture() = default;

	virtual void Draw(CDC* pDC) = 0;
	virtual CRect GetBoundingBox() const = 0;

	CPoint GetPosition() const { return m_leftTop; }
	void SetPosition(const CPoint& leftTop) { m_leftTop = leftTop; }

	void Offset(int dx, int dy)
	{
		m_leftTop.x += dx;
		m_leftTop.y += dy;
	}

protected:
	CPoint m_leftTop;
};

class LodgePicture : public SimplePicture
{
public:
	LodgePicture(int x = 0, int y = 0) :
		SimplePicture(x, y)
	{
		m_width = 0;
		m_height = 0;
	}

	void Draw(CDC* pDC) 
	{
	
	}

	CRect GetBoundingBox() const override 
	{
		return {};
	}


private:
	int m_width;
	int m_height;
};