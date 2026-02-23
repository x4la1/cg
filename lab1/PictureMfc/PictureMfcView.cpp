
// PictureMfcView.cpp: реализация класса CPictureMfcView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "PictureMfc.h"
#endif

#include "PictureMfcDoc.h"
#include "PictureMfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPictureMfcView

IMPLEMENT_DYNCREATE(CPictureMfcView, CView)

BEGIN_MESSAGE_MAP(CPictureMfcView, CView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CPictureMfcView

CPictureMfcView::CPictureMfcView() noexcept
{
	m_picture = std::make_unique<LodgePicture>(100, 100);

}

CPictureMfcView::~CPictureMfcView()
{
}

BOOL CPictureMfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CPictureMfcView

void CPictureMfcView::OnDraw(CDC* /*pDC*/)
{
	CPictureMfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
}


// Диагностика CPictureMfcView

#ifdef _DEBUG
void CPictureMfcView::AssertValid() const
{
	CView::AssertValid();
}

void CPictureMfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPictureMfcDoc* CPictureMfcView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPictureMfcDoc)));
	return (CPictureMfcDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CPictureMfcView
