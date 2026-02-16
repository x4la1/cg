
// PictureMfcView.h: интерфейс класса CPictureMfcView
//

#pragma once


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
};

#ifndef _DEBUG  // версия отладки в PictureMfcView.cpp
inline CPictureMfcDoc* CPictureMfcView::GetDocument() const
   { return reinterpret_cast<CPictureMfcDoc*>(m_pDocument); }
#endif

