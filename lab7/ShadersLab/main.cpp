#pragma once
#include "pch.h"
#include "MyApplication.h"
#include "GdiInitializer.h"

int main()
{
	GdiplusInitializer gdiInitializer;

	MyApplication app;
	
	glewInit();
	if (!GLEW_ARB_shader_objects)
	{
		return -1;
	}
	
	app.MainLoop();
	return 0; 
}