#pragma once
#include "pch.h"
#include "MyApplication.h"
#include "GdiInitializer.h"

int main()
{
	GdiplusInitializer gdiInitializer;

	MyApplication app;

	glewInit();

	app.MainLoop();
	return 0;
}