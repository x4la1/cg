
// ImageViewer.h: основной файл заголовка для приложения ImageViewer
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы
#include "GdiPlusInitializer.h"

// CImageViewerApp:
// Сведения о реализации этого класса: ImageViewer.cpp
//

class CImageViewerApp : public CWinApp
{
public:
	CImageViewerApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

protected:
	std::unique_ptr<GdiPlusInitializer> m_initializer;
};

extern CImageViewerApp theApp;
