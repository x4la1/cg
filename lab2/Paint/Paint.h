
// Paint.h: основной файл заголовка для приложения Paint
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы
#include "GdiPlusInitializer.h"

// CPaintApp:
// Сведения о реализации этого класса: Paint.cpp
//

class CPaintApp : public CWinApp
{
public:
	CPaintApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	std::unique_ptr<GdiPlusInitializer> m_initializer;
};

extern CPaintApp theApp;
