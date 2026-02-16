
// PictureMfc.h: основной файл заголовка для приложения PictureMfc
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CPictureMfcApp:
// Сведения о реализации этого класса: PictureMfc.cpp
//

class CPictureMfcApp : public CWinApp
{
public:
	CPictureMfcApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPictureMfcApp theApp;
