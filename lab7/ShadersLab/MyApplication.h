#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "RotationContoller.h"
#include "DirectLight.h"
#include "ProgramBase.h"
#include "ShaderLoader.h"
#include "ShaderCompiler.h"
#include "ProgramLinker.h"

const double FIELD_OF_VIEW = 60;
const double ZNEAR = 1;
const double ZFAR = 5;

class MyApplication : public GLApplication
{
public:
	MyApplication() :
		GLApplication("", 800, 600, true, false),
		m_rotationController(800, 600)
	{
	}

	~MyApplication()
	{
		glDeleteProgram(m_program);
	}

protected:
	void OnInit() override
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.5, 0.5, 0.5, 1);
		glColor3f(1, 1, 1);

		glLoadIdentity();
		gluLookAt(
			0, 0, 3,
			0, 0, 0,
			0, 1, 0);

		InitShaders();
	}

	void OnDisplay() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_program);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(-0.8, -0.8);

			glTexCoord2f(4, 0);
			glVertex2f(0.8, -0.8);

			glTexCoord2f(4, 4);
			glVertex2f(0.8, 0.8);

			glTexCoord2f(0, 4);
			glVertex2f(-0.8, 0.8);
		}
		glEnd();

		glUseProgram(0);
	}

	void OnMouse(int button, int state, int x, int y) override
	{
		m_rotationController.OnMouse(button, state, x, y);
	}

	void OnMotion(int x, int y) override
	{
		if (m_rotationController.LeftButtonIsPressed())
		{
			m_rotationController.OnMotion(x, y);
			PostRedisplay();
		}
	}

	void OnReshape(int width, int height) override
	{
		glViewport(0, 0, width, height);

		double aspect = double(width) / double(height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-aspect, +aspect, -1, 1, 0, 10);
		glMatrixMode(GL_MODELVIEW);
	}

	void OnIdle() override
	{
		PostRedisplay();
	}

	void InitShaders()
	{
		// Создаем загрузчик шейдеров
		ShaderLoader loader;
		// И загружаем с его помощью вершинный и фрагментный шейдеры
		Shader vertexShader =
			loader.LoadShader(GL_VERTEX_SHADER, "checkers.vsh");
		Shader fragmentShader =
			loader.LoadShader(GL_FRAGMENT_SHADER, "checkers.fsh");

		// Создаем компилятор
		ShaderCompiler compiler;

		// и запускаем компиляцию шейдеров
		compiler.CompileShader(vertexShader);
		compiler.CompileShader(fragmentShader);

		// Создаем программу и присоединяем к ней шейдеры
		m_program.Create();
		m_program.AttachShader(vertexShader);
		m_program.AttachShader(fragmentShader);

		// Проверяем состояние скомпилированности шейдеров.
		// Если хотя бы один из шейдеров скомпилировался с ошибкой
		// будет выброшено исключение
		compiler.CheckStatus();
		// Все нормально, шейдеры скомпилировались без проблем

		// Создаем компоновщик,
		ProgramLinker linker;

		// компонуем программу с его помощью
		linker.LinkProgram(m_program);

		// Проверяем состояние скомпонованности программ
		// Если при компоновке возникла ошибка, то
		// будет выброшено исключение
		linker.CheckStatus();
		// Все нормально
	}

private:
	RotationController m_rotationController;
	Program m_program;
};
