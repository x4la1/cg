#pragma once
#include "pch.h"
#include "MyApplication.h"
#include "Model.h"
#include "GdiInitializer.h"




int main(int argc, char** argv)
{
	GdiplusInitializer init;
	MyApplication app;
	glewInit();
	app.MainLoop();
	return 0;
}