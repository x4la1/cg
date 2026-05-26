#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "ProgramBase.h"
#include "ShaderLoader.h"
#include "ShaderCompiler.h"
#include "ProgramLinker.h"

class FlagApplication : public GLApplication
{
public:
	FlagApplication() :
		GLApplication("Flag", 800, 600, true, false)
	{
	}

	~FlagApplication()
	{
		glDeleteProgram(m_program);
	}

protected:
	void OnInit() override
	{
		glClearColor(0.5, 0.5, 0.5, 1);
		glColor3f(1, 1, 1);

		InitShaders();
	}

	void OnDisplay() override
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_program);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(-1.0f, -1.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(1.0f, -1.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(1.0f, 1.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(-1.0f, 1.0f);
		}
		glEnd();

		glUseProgram(0);
	}

	void OnReshape(int width, int height) override
	{
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void InitShaders()
	{
		ShaderLoader loader;
		Shader fragmentShader = loader.LoadShader(GL_FRAGMENT_SHADER, L"shaders/flag.fsh");

		m_program.Create();
		m_program.AttachShader(fragmentShader);

		ShaderCompiler compiler;
		compiler.CompileShader(fragmentShader);
		compiler.CheckStatus();

		ProgramLinker linker;
		linker.LinkProgram(m_program);
		linker.CheckStatus();
	}

private:
	Program m_program;
};