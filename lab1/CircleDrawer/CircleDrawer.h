
// CircleDrawer.h: основной файл заголовка для приложения CircleDrawer
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CCircleDrawerApp:
// Сведения о реализации этого класса: CircleDrawer.cpp
//

class CCircleDrawerApp : public CWinApp
{
public:
	CCircleDrawerApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCircleDrawerApp theApp;
