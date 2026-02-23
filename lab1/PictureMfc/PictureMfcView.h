
// PictureMfcView.h: интерфейс класса CPictureMfcView
//

#pragma once
#include "Picture.h"
#include <memory>

class CPictureMfcView : public CView
{
protected: // создать только из сериализации
	CPictureMfcView() noexcept;
	DECLARE_DYNCREATE(CPictureMfcView)

// Атрибуты
public:
	CPictureMfcDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CPictureMfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

private:
	std::unique_ptr<Picture> m_picture;
};

#ifndef _DEBUG  // версия отладки в PictureMfcView.cpp
inline CPictureMfcDoc* CPictureMfcView::GetDocument() const
   { return reinterpret_cast<CPictureMfcDoc*>(m_pDocument); }
#endif

