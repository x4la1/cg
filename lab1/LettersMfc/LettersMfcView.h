
// LettersMfcView.h: интерфейс класса CLettersMfcView
//

#pragma once
#include "Letter.h"
#include <vector>
#include <memory>

class CLettersMfcView : public CView
{
protected: // создать только из сериализации
	CLettersMfcView() noexcept;
	DECLARE_DYNCREATE(CLettersMfcView)

// Атрибуты
public:
	CLettersMfcDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate();
	virtual void OnDestroy();
	virtual void OnTimer(UINT_PTR nIDEvent);

// Реализация
public:
	virtual ~CLettersMfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

private:
	void CreateLetters();
	std::unique_ptr<Letter> m_letterL;
	std::unique_ptr<Letter> m_letterD;
	std::unique_ptr<Letter> m_letterA;
	GdiResources m_gdiResources;
	DWORD m_lastTick;
	double m_animationTime;
};

#ifndef _DEBUG  // версия отладки в LettersMfcView.cpp
inline CLettersMfcDoc* CLettersMfcView::GetDocument() const
   { return reinterpret_cast<CLettersMfcDoc*>(m_pDocument); }
#endif

