#pragma once
#include "pch.h"
#include "MazeApplication.h"
#include "GdiInitializer.h"


int main()
{
	GdiplusInitializer gdiInitializer;
	MazeApplication app;
	app.MainLoop();

	return 0;
}

