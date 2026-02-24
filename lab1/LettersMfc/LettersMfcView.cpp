
// LettersMfcView.cpp: реализация класса CLettersMfcView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "LettersMfc.h"
#endif

#include "LettersMfcDoc.h"
#include "LettersMfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const UINT_PTR ANIMATION_TIMER_ID = 1;
const int LETTER_SIZE = 150;
// CLettersMfcView

IMPLEMENT_DYNCREATE(CLettersMfcView, CView)

BEGIN_MESSAGE_MAP(CLettersMfcView, CView)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// Создание или уничтожение CLettersMfcView

CLettersMfcView::CLettersMfcView() noexcept
{
	m_animationTime = 0;
	m_lastTick = 0;
}

CLettersMfcView::~CLettersMfcView()
{
}

BOOL CLettersMfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CLettersMfcView

void CLettersMfcView::OnDraw(CDC* pDC)
{
	CLettersMfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, RGB(0, 0, 0));

	int centerX = rect.Width() / 2;
	int centerY = rect.Height() / 2;

	m_letterL->Draw(pDC, m_gdiResources, m_animationTime, centerX - LETTER_SIZE * 1.5, centerY);
	m_letterD->Draw(pDC, m_gdiResources, m_animationTime, centerX, centerY);
	m_letterA->Draw(pDC, m_gdiResources, m_animationTime, centerX + LETTER_SIZE * 1.5, centerY);
}

void CLettersMfcView::CreateLetters()
{
	m_letterL = std::make_unique<LetterL>(LETTER_SIZE, 0.0);
	m_letterD = std::make_unique<LetterD>(LETTER_SIZE, 0.3);
	m_letterA = std::make_unique<LetterA>(LETTER_SIZE, 0.6);
}

void CLettersMfcView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_gdiResources.whitePen.CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	m_gdiResources.whiteBrush.CreateSolidBrush(RGB(255, 255, 255));
	m_gdiResources.bluePen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	m_gdiResources.blueBrush.CreateSolidBrush(RGB(0, 0, 255));
	m_gdiResources.redPen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_gdiResources.redBrush.CreateSolidBrush(RGB(255, 0, 0));

	CreateLetters();
	SetTimer(ANIMATION_TIMER_ID, 20, NULL);
}

void CLettersMfcView::OnDestroy()
{
	KillTimer(ANIMATION_TIMER_ID);
	DeleteObject(m_gdiResources.whitePen);
	DeleteObject(m_gdiResources.whiteBrush);
	DeleteObject(m_gdiResources.bluePen);
	DeleteObject(m_gdiResources.bluePen);
	DeleteObject(m_gdiResources.redPen);
	DeleteObject(m_gdiResources.redPen);

	CView::OnDestroy();
}

void CLettersMfcView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ANIMATION_TIMER_ID)
	{
		DWORD currentTick = GetTickCount64();
		int delta = currentTick - m_lastTick;
		m_lastTick = currentTick;

		double deltaSeconds = delta * 0.001;
		m_animationTime += deltaSeconds;
		
		Invalidate();
	}
	else
	{
		CView::OnTimer(nIDEvent);
	}
}

// Диагностика CLettersMfcView

#ifdef _DEBUG
void CLettersMfcView::AssertValid() const
{
	CView::AssertValid();
}

void CLettersMfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLettersMfcDoc* CLettersMfcView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLettersMfcDoc)));
	return (CLettersMfcDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CLettersMfcView
