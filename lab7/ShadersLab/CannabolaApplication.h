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

class CannabolaApplication : public GLApplication
{
public:
	CannabolaApplication() :
		GLApplication("", 800, 600, true, false)
	{
	}

	~CannabolaApplication()
	{
		glDeleteProgram(m_program);
	}

protected:
	void OnInit() override
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glColor3f(0.0f, 0.5f, 0.0f);

		InitShaders();

		int numSteps = 2000;
		float step = 2.0 * 3.14159265358979323846f / numSteps;

		std::vector<glm::fvec3> vertices;
		vertices.reserve(numSteps + 1);

		for (int i = 0; i <= numSteps; ++i)
		{
			vertices.push_back(glm::fvec3(i * step, 0.0f, 0.0f));
		}
		m_vertexCount = vertices.size();

		m_vertexBuffer.Create();
		m_vertexBuffer.BufferData(
			vertices.size() * sizeof(glm::fvec3),
			&vertices[0],
			GL_STATIC_DRAW
		);
	}

	void OnDisplay() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_program);

		m_vertexBuffer.Bind();
		glVertexPointer(3, GL_FLOAT, sizeof(glm::fvec3), m_vertexBuffer.GetBufferPointer());
		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);

		glDisableClientState(GL_VERTEX_ARRAY);
		glUseProgram(0);
	}

	void OnMouse(int button, int state, int x, int y) override
	{
	}

	void OnMotion(int x, int y) override
	{
	}

	void OnReshape(int width, int height) override
	{
		glViewport(0, 0, width, height);

		double aspect = double(width) / double(height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (aspect >= 1.0)
		{
			glOrtho(-3.0 * aspect, 3.0 * aspect, -3.0, 3.0, -1.0, 1.0);
		}
		else
		{
			glOrtho(-3.0, 3.0, -3.0 / aspect, 3.0 / aspect, -1.0, 1.0);
		}

		glMatrixMode(GL_MODELVIEW);
	}

	void OnIdle() override
	{
	}

	void InitShaders()
	{
		ShaderLoader loader;
		Shader vertexShader = loader.LoadShader(GL_VERTEX_SHADER, L"shaders/cannabola.vsh");

		m_program.Create();
		m_program.AttachShader(vertexShader);

		ShaderCompiler compiler;
		compiler.CompileShader(vertexShader);
		compiler.CheckStatus();

		ProgramLinker linker;
		linker.LinkProgram(m_program);
		linker.CheckStatus();
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
	Program m_program;
	size_t m_vertexCount;
	VertexBuffer m_vertexBuffer;
};
