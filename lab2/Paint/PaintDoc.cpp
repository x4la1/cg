
// PaintDoc.cpp: реализация класса CPaintDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Paint.h"
#endif

#include "PaintDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPaintDoc

IMPLEMENT_DYNCREATE(CPaintDoc, CDocument)

BEGIN_MESSAGE_MAP(CPaintDoc, CDocument)
END_MESSAGE_MAP()


// Создание или уничтожение CPaintDoc

CPaintDoc::CPaintDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CPaintDoc::~CPaintDoc()
{
}

BOOL CPaintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_bitmap = std::make_unique<Gdiplus::Bitmap>(1400, 650, PixelFormat32bppRGB);
	Gdiplus::Graphics g(m_bitmap.get());
	g.Clear(Gdiplus::Color(255, 255, 255));
	UpdateAllViews(NULL);

	return TRUE;
}

BOOL CPaintDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	Gdiplus::Image img(lpszPathName);
	if (img.GetLastStatus() != Gdiplus::Ok)
	{
		return FALSE;
	}

	GUID format;
	img.GetRawFormat(&format);

	if (format != Gdiplus::ImageFormatJPEG)
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

BOOL CPaintDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (!m_bitmap)
		return FALSE;

	CString path(lpszPathName);
	CString ext = path.Right(4).MakeLower();
	if (ext != _T(".jpg"))
	{
		AfxMessageBox(_T("Only JPEG format allowed"));
		return FALSE;
	}

	CLSID encoderClsid = GetEncoderClsid(_T("image/jpeg"));
	if (encoderClsid == CLSID_NULL)
	{
		AfxMessageBox(_T("JPEG codec not found"));
		return FALSE;
	}

	Gdiplus::EncoderParameters encoderParams;
	encoderParams.Count = 1;
	encoderParams.Parameter[0].Guid = Gdiplus::EncoderQuality;
	encoderParams.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
	encoderParams.Parameter[0].NumberOfValues = 1;
	LONG quality = 100;
	encoderParams.Parameter[0].Value = &quality;

	Gdiplus::Status status = m_bitmap->Save(lpszPathName, &encoderClsid, &encoderParams);
	if (status != Gdiplus::Ok)
	{
		AfxMessageBox(_T("Ошибка сохранения файла"));
		return FALSE;
	}
	return TRUE;
}

CLSID CPaintDoc::GetEncoderClsid(const WCHAR* format) const
{
	UINT num = 0;
	UINT size = 0;
	Gdiplus::GetImageEncodersSize(&num, &size);

	std::vector<BYTE> buffer(size);
	Gdiplus::ImageCodecInfo* pImageCodecInfo = reinterpret_cast<Gdiplus::ImageCodecInfo*>(buffer.data());
	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT i = 0; i < num; ++i)
	{
		if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			return pImageCodecInfo[i].Clsid;
		}
	}

	return CLSID_NULL;
}

void CPaintDoc::DeleteContents()
{
	m_bitmap.reset();
	CDocument::DeleteContents();
}

void CPaintDoc::DrawLine(const Gdiplus::Point& p1, const Gdiplus::Point& p2,
	Gdiplus::Color color, float width)
{
	if (!m_bitmap) return;

	Gdiplus::Graphics g(m_bitmap.get());
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	Gdiplus::Pen pen(color, width);
	g.DrawLine(&pen, p1, p2);
}

// Сериализация CPaintDoc

void CPaintDoc::Serialize(CArchive& ar)
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

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CPaintDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CPaintDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CPaintDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CPaintDoc

#ifdef _DEBUG
void CPaintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPaintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CPaintDoc