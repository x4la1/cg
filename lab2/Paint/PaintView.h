
// PaintView.h: интерфейс класса CPaintView
//

#pragma once


class CPaintView : public CView
{
protected: // создать только из сериализации
	CPaintView() noexcept;
	DECLARE_DYNCREATE(CPaintView)

	// Атрибуты
public:
	CPaintDoc* GetDocument() const;

	// Операции
public:

	// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual void OnInitialUpdate();
	virtual void OnColorPick();
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	// Реализация
public:
	virtual ~CPaintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool m_isDrawing;
	Gdiplus::Point m_prevPoint;
	Gdiplus::Color m_penColor;
	float m_penWidth;

	// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в PaintView.cpp
inline CPaintDoc* CPaintView::GetDocument() const
{
	return reinterpret_cast<CPaintDoc*>(m_pDocument);
}
#endif

