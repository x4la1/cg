#pragma once
#include "pch.h"
#include "GdiInitializer.h"
#include "SphereMorphApplication.h"

int main()
{
	GdiplusInitializer gdiInitializer;

	SphereMorphApplication app;
	
	glewInit();
	if (!GLEW_ARB_shader_objects)
	{
		return -1;
	}

	GLint maxVertexUniforms;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniforms);
	std::cout << "The implementation supports as many as "
		<< maxVertexUniforms << " vertex uniforms\n";

	GLint maxFragmentUniforms;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniforms);
	std::cout << "The implementation supports as many as "
		<< maxFragmentUniforms << " fragment uniforms\n";
	
	app.MainLoop();
	return 0; 
}