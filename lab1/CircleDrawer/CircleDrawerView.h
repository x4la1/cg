
// CircleDrawerView.h: интерфейс класса CCircleDrawerView
//

#pragma once
#include <memory>
#include "Circle.h"

class CCircleDrawerView : public CView
{
protected: // создать только из сериализации
	CCircleDrawerView() noexcept;
	DECLARE_DYNCREATE(CCircleDrawerView)

// Атрибуты
public:
	CCircleDrawerDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDestroy();
protected:

// Реализация
public:
	virtual ~CCircleDrawerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

private:
	std::unique_ptr<Circle> m_circle;
};

#ifndef _DEBUG  // версия отладки в CircleDrawerView.cpp
inline CCircleDrawerDoc* CCircleDrawerView::GetDocument() const
   { return reinterpret_cast<CCircleDrawerDoc*>(m_pDocument); }
#endif

