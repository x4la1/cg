#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "RotationContoller.h"
#include "DirectLight.h"
#include "ProgramBase.h"
#include "ShaderLoader.h"
#include "ShaderCompiler.h"
#include "ProgramLinker.h"
#include "ProgramInfo.h"
#include "MorphShape.h"
#include "AnimationContoller.h"

const double FIELD_OF_VIEW = 60;
const double ZNEAR = 1;
const double ZFAR = 5;

const int NUMBER_OF_VERTICES = 360;
const float SHAPE_MAX_RADIUS = 1.0f;
const float SHAPE_MIN_RADIUS = 0.5f;
const float ANIMATION_PERIOD = 2.0f;

class MyApplication : public GLApplication
{
public:
	MyApplication() :
		GLApplication("", 800, 600, true, false),
		m_morphShape(NUMBER_OF_VERTICES, SHAPE_MAX_RADIUS, SHAPE_MIN_RADIUS),
		m_phase(0),
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
		glClearColor(1, 1, 1, 1);
		glColor3f(0, 0, 0);

		glLoadIdentity();
		InitShaders();

		// Получаем расположение uniform и attribute-переменных
		GLint phaseLocation = m_program.GetUniformLocation("phase");
		GLint vertex2Location = m_program.GetAttribLocation("endPosition");;
		// Передаем информацию о программе и ее переменных фигуре
		m_morphShape.SetProgram(m_program, phaseLocation, vertex2Location);
	}

	void OnDisplay() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_morphShape.Draw((sinf(m_phase) + 1) / 2);
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

		// Вычисляем соотношение сторон клиентской области окна
		double aspect = double(width) / double(height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(
			-SHAPE_MAX_RADIUS * aspect, SHAPE_MAX_RADIUS * aspect,
			-SHAPE_MAX_RADIUS, SHAPE_MAX_RADIUS,
			-1, +1);
		glMatrixMode(GL_MODELVIEW);
	}

	void OnIdle() override
	{
		m_animationController.Tick();

		float delta = m_animationController.GetTimeDelta() * 0.001f;

		m_phase = fmodf(
			float(m_phase + delta * 2 * 3.14159265358979323846 / ANIMATION_PERIOD),
			float(2 * 3.14159265358979323846)
		);

		PostRedisplay();
	}

	void InitShaders()
	{
		// Загружаем шейдеры
		ShaderLoader loader;
		Shader vertexShader =
			loader.LoadShader(GL_VERTEX_SHADER, L"shaders/vertex_blend.vsh");

		// Создаем программы и присоединяем к ней шейдеры
		m_program.Create();
		m_program.AttachShader(vertexShader);

		// Компилируем шейдеры
		ShaderCompiler compiler;
		compiler.CompileShader(vertexShader);
		compiler.CheckStatus();

		// Компонуем программу
		ProgramLinker linker;
		linker.LinkProgram(m_program);
		linker.CheckStatus();

		// Выводим информацию о программе
		ProgramInfo programInfo(m_program);
		programInfo.Print(std::cout);
	}

	void PrintProgramInfo(GLuint programId)
	{
		ProgramInfo programInfo(programId);

		std::cout << "Program id: " << programId << "\n";

		GLuint const activeUniforms = programInfo.GetActiveUniforms();
		std::cout << "  Active uniforms: " << activeUniforms << "\n";
		for (GLuint uniform = 0; uniform < activeUniforms; ++uniform)
		{
			std::cout << "    ";
			programInfo.PrintUniformInfo(uniform, std::cout);
			std::cout << "\n";
		}
	}

private:
	RotationController m_rotationController;
	AnimationController m_animationController;
	Program m_program;
	MorphShape m_morphShape;
	float m_phase;
};
