
// TestMfcView.h: интерфейс класса CTestMfcView
//

#pragma once


class CTestMfcView : public CView
{
protected: // создать только из сериализации
	CTestMfcView() noexcept;
	DECLARE_DYNCREATE(CTestMfcView)

// Атрибуты
public:
	CTestMfcDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CTestMfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в TestMfcView.cpp
inline CTestMfcDoc* CTestMfcView::GetDocument() const
   { return reinterpret_cast<CTestMfcDoc*>(m_pDocument); }
#endif

