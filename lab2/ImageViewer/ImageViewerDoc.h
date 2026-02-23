
// ImageViewerDoc.h: интерфейс класса CImageViewerDoc 
//


#pragma once


class CImageViewerDoc : public CDocument
{
protected: // создать только из сериализации
	CImageViewerDoc() noexcept;
	DECLARE_DYNCREATE(CImageViewerDoc)

	// Атрибуты
public:

	// Операции
public:

	// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;
	virtual void DeleteContents() override;
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

private:
	std::unique_ptr<Gdiplus::Bitmap> m_bitmap;

	// Реализация
public:
	virtual ~CImageViewerDoc();
	Gdiplus::Bitmap* GetBitmap() const { return m_bitmap.get(); }
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:

	// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
