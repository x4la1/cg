
// TestMfc.h: основной файл заголовка для приложения TestMfc
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// TestMfcApp:
// Сведения о реализации этого класса: TestMfc.cpp
//

class TestMfcApp : public CWinApp
{
public:
	TestMfcApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern TestMfcApp theApp;
