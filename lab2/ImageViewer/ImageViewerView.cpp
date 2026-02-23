
// ImageViewerView.cpp: реализация класса CImageViewerView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "ImageViewer.h"
#endif

#include "ImageViewerDoc.h"
#include "ImageViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageViewerView

IMPLEMENT_DYNCREATE(CImageViewerView, CView)

BEGIN_MESSAGE_MAP(CImageViewerView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Создание или уничтожение CImageViewerView

CImageViewerView::CImageViewerView() noexcept
{
	m_isDragging = false;
}

CImageViewerView::~CImageViewerView()
{
}

BOOL CImageViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs 

	return CView::PreCreateWindow(cs);
}

// Рисование CImageViewerView

void CImageViewerView::OnDraw(CDC* pDC)
{
	CImageViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	CMemDC memDC(*pDC, this);
	CDC* dc = &memDC.GetDC();

	dc->FillSolidRect(&clientRect, RGB(0, 0, 0));

	Gdiplus::Bitmap* pBitmap = pDoc->GetBitmap();
	if (pBitmap)
	{
		Gdiplus::Graphics g(dc->GetSafeHdc());
		g.DrawImage(
			pBitmap,
			Gdiplus::Rect(
				m_imageRect.left, m_imageRect.top,
				m_imageRect.Width(), m_imageRect.Height()
			),
			0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(),
			Gdiplus::UnitPixel
		);
	}
}

void CImageViewerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	Gdiplus::Bitmap* pBitmap = GetDocument()->GetBitmap();
	if (pBitmap)
	{
		CRect client;
		GetClientRect(&client);
		int width = pBitmap->GetWidth();
		int height = pBitmap->GetHeight();
		int x = (client.Width() - width) / 2;
		int y = (client.Height() - height) / 2;
		m_imageRect.SetRect(x, y, x + width, y + height);
	}
	Invalidate();
}

void CImageViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetDocument()->GetBitmap() && m_imageRect.PtInRect(point))
	{
		m_isDragging = true;
		m_dragStart = point;
		m_offsetStart = m_imageRect.TopLeft();
		SetCapture();
	}
	else
	{
		CView::OnLButtonDown(nFlags, point);
	}
}

void CImageViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_isDragging)
	{
		CRect clientRect;
		GetClientRect(&clientRect);

		CPoint delta = point - m_dragStart;
		CPoint newTopLeft = m_offsetStart + delta;

		if (newTopLeft.x > 0 && newTopLeft.x + m_imageRect.Width() <= clientRect.Width()
			&& newTopLeft.y > 0 && newTopLeft.y + m_imageRect.Height() <= clientRect.Height())
		{
			m_imageRect.MoveToXY(newTopLeft.x, newTopLeft.y);
			Invalidate();
		}
	}
	else
	{
		CView::OnMouseMove(nFlags, point);
	}
}

void CImageViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_isDragging)
	{
		m_isDragging = false;
		ReleaseCapture();
	}
	CView::OnLButtonUp(nFlags, point);
}

BOOL CImageViewerView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

// Диагностика CImageViewerView

#ifdef _DEBUG
void CImageViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CImageViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageViewerDoc* CImageViewerView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageViewerDoc)));
	return (CImageViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CImageViewerView
