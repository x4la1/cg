#pragma once
#include "pch.h"

class GdiPlusInitializer
{
public:
	GdiPlusInitializer()
	{
		Gdiplus::GdiplusStartupInput input;
		Gdiplus::GdiplusStartupOutput output;

		if (Gdiplus::GdiplusStartup(&m_token, &input, &output) != Gdiplus::Ok)
		{
			throw std::runtime_error("Failed to initialize GDI+");
		}
	}

	~GdiPlusInitializer()
	{
		Gdiplus::GdiplusShutdown(m_token);
	}

private:
	ULONG_PTR m_token;
};