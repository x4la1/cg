
// ImageViewerDoc.cpp: реализация класса CImageViewerDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "ImageViewer.h"
#endif

#include "ImageViewerDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageViewerDoc

IMPLEMENT_DYNCREATE(CImageViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageViewerDoc, CDocument)
END_MESSAGE_MAP()


// Создание или уничтожение CImageViewerDoc

CImageViewerDoc::CImageViewerDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CImageViewerDoc::~CImageViewerDoc()
{
}

BOOL CImageViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CImageViewerDoc

void CImageViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

BOOL CImageViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	Gdiplus::Image img(lpszPathName);
	if (img.GetLastStatus() != Gdiplus::Ok)
	{
		AfxMessageBox(_T("File is not image"));
		return FALSE;
	}

	auto format = std::make_unique<GUID>();

	img.GetRawFormat(format.get());

	if (*format != Gdiplus::ImageFormatJPEG)
	{
		AfxMessageBox(_T("Only .jpeg format allowed."));
		return FALSE;
	}

	auto bitmap = std::make_unique<Gdiplus::Bitmap>(
		img.GetWidth(), img.GetHeight(), PixelFormat32bppARGB
	);

	Gdiplus::Graphics g(bitmap.get());
	g.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight());

	m_bitmap = std::move(bitmap);
	UpdateAllViews(NULL);

	return TRUE;
}

void CImageViewerDoc::DeleteContents()
{
	m_bitmap.reset();
	CDocument::DeleteContents();
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CImageViewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CImageViewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CImageViewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CImageViewerDoc

#ifdef _DEBUG
void CImageViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CImageViewerDoc
