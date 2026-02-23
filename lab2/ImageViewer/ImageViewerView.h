
// ImageViewerView.h: интерфейс класса CImageViewerView
//

#pragma once


class CImageViewerView : public CView
{
protected: // создать только из сериализации
	CImageViewerView() noexcept;
	DECLARE_DYNCREATE(CImageViewerView)

// Атрибуты
public:
	CImageViewerDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CImageViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CRect m_imageRect;
	BOOL m_isDragging;
	CPoint m_dragStart;
	CPoint m_offsetStart;

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в ImageViewerView.cpp
inline CImageViewerDoc* CImageViewerView::GetDocument() const
   { return reinterpret_cast<CImageViewerDoc*>(m_pDocument); }
#endif

