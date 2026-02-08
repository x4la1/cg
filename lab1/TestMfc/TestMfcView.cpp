
// TestMfcView.cpp: реализация класса CTestMfcView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "TestMfc.h"
#endif

#include "TestMfcDoc.h"
#include "TestMfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMfcView

IMPLEMENT_DYNCREATE(CTestMfcView, CView)

BEGIN_MESSAGE_MAP(CTestMfcView, CView)
END_MESSAGE_MAP()

// Создание или уничтожение CTestMfcView

CTestMfcView::CTestMfcView() noexcept
{
	// TODO: добавьте код создания

}

CTestMfcView::~CTestMfcView()
{
}

BOOL CTestMfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CTestMfcView

void CTestMfcView::OnDraw(CDC* /*pDC*/)
{
	CTestMfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
}


// Диагностика CTestMfcView

#ifdef _DEBUG
void CTestMfcView::AssertValid() const
{
	CView::AssertValid();
}

void CTestMfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestMfcDoc* CTestMfcView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestMfcDoc)));
	return (CTestMfcDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CTestMfcView
