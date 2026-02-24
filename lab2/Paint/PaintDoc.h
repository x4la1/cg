
// PaintDoc.h: интерфейс класса CPaintDoc 
//


#pragma once


class CPaintDoc : public CDocument
{
protected: // создать только из сериализации
	CPaintDoc() noexcept;
	DECLARE_DYNCREATE(CPaintDoc)

// Атрибуты
public:

// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument() override;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) override;
	virtual void DeleteContents() override;

	Gdiplus::Bitmap* GetBitmap() const { return m_bitmap.get(); }

	void DrawLine(const Gdiplus::Point& p1, const Gdiplus::Point& p2,
		Gdiplus::Color color, float width);

	virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CPaintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CLSID GetEncoderClsid(const WCHAR* format) const;

	std::unique_ptr<Gdiplus::Bitmap> m_bitmap;

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
