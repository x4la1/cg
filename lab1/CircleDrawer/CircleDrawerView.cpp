
// CircleDrawerView.cpp: реализация класса CCircleDrawerView
//
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "CircleDrawer.h"
#endif

#include "CircleDrawerDoc.h"
#include "CircleDrawerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCircleDrawerView

IMPLEMENT_DYNCREATE(CCircleDrawerView, CView)

BEGIN_MESSAGE_MAP(CCircleDrawerView, CView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// Создание или уничтожение CCircleDrawerView

CCircleDrawerView::CCircleDrawerView() noexcept
{
	m_circle = std::make_unique<Circle>(
		400,
		300,
		100,
		RGB(0, 0, 0),
		RGB(0, 255, 0),
		1
	);
}

CCircleDrawerView::~CCircleDrawerView()
{
}

BOOL CCircleDrawerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CCircleDrawerView

void CCircleDrawerView::OnDraw(CDC* pDC)
{
	CCircleDrawerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	m_circle->Draw(pDC);
}

void CCircleDrawerView::OnDestroy()
{
	CView::OnDestroy();
}

// Диагностика CCircleDrawerView

#ifdef _DEBUG
void CCircleDrawerView::AssertValid() const
{
	CView::AssertValid();
}

void CCircleDrawerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCircleDrawerDoc* CCircleDrawerView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCircleDrawerDoc)));
	return (CCircleDrawerDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CCircleDrawerView
