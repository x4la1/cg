
// PaintView.cpp: реализация класса CPaintView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Paint.h"
#endif

#include "PaintDoc.h"
#include "PaintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPaintView

IMPLEMENT_DYNCREATE(CPaintView, CView)

BEGIN_MESSAGE_MAP(CPaintView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CPaintView

CPaintView::CPaintView() noexcept :
	m_isDrawing(false)
	, m_penColor(0, 0, 0)
	, m_penWidth(3.0f)
{
}

CPaintView::~CPaintView()
{
}

BOOL CPaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CPaintView

void CPaintView::OnSize(UINT nType, int cx, int cy)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	CView::OnSize(nType, cx, cy);
	Invalidate();
}

BOOL CPaintView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CPaintView::OnDraw(CDC* pDC)
{
	CPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	
	CMemDC memDC(*pDC, this);
	CDC* dc = &memDC.GetDC();

	dc->FillSolidRect(&clientRect, RGB(125, 125, 125));

	Gdiplus::Bitmap* bitmap = pDoc->GetBitmap();
	if (bitmap)
	{
		Gdiplus::Graphics g(dc->GetSafeHdc());
		g.DrawImage(
			bitmap,
			0, 0,
			bitmap->GetWidth(), bitmap->GetHeight()
		);
	}
}

void CPaintView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CPaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPaintDoc* pDoc = GetDocument();

	Gdiplus::Bitmap* bitmap = pDoc->GetBitmap();
	if (point.x >= 0 && point.x < bitmap->GetWidth() &&
		point.y >= 0 && point.y < bitmap->GetHeight())
	{
		m_isDrawing = true;
		m_prevPoint = Gdiplus::Point(point.x, point.y);
		SetCapture();

		pDoc->DrawLine(m_prevPoint, m_prevPoint, m_penColor, m_penWidth);
		Invalidate();
	}
	else
	{
		CView::OnLButtonDown(nFlags, point);
	}
}

void CPaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_isDrawing)
	{
		CPaintDoc* pDoc = GetDocument();
		Gdiplus::Bitmap* bitmap = pDoc->GetBitmap();
		if (point.x > 0 && point.x < bitmap->GetWidth()
			&& point.y > 0 && point.y < bitmap->GetHeight())
		{
			Gdiplus::Point currPoint(point.x, point.y);

			if (!currPoint.Equals(m_prevPoint))
			{
				pDoc->DrawLine(m_prevPoint, currPoint, m_penColor, m_penWidth);
				m_prevPoint = currPoint;
				Invalidate();
			}
		}
	}
	else
	{
		CView::OnMouseMove(nFlags, point);
	}
}

void CPaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_isDrawing)
	{
		m_isDrawing = false;
		ReleaseCapture();
	}
	CView::OnLButtonUp(nFlags, point);
}


// Диагностика CPaintView

#ifdef _DEBUG
void CPaintView::AssertValid() const
{
	CView::AssertValid();
}

void CPaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPaintDoc* CPaintView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaintDoc)));
	return (CPaintDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CPaintView
