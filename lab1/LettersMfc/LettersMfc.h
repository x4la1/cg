
// LettersMfc.h: основной файл заголовка для приложения LettersMfc
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CLettersMfcApp:
// Сведения о реализации этого класса: LettersMfc.cpp
//

class CLettersMfcApp : public CWinApp
{
public:
	CLettersMfcApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLettersMfcApp theApp;
