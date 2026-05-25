#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "ProgramBase.h"
#include "ShaderLoader.h"
#include "ShaderCompiler.h"
#include "ProgramLinker.h"
#include "Buffer.h"
#include "RotationContoller.h"
#include "AnimationContoller.h"

class SphereMorphApplication : public GLApplication
{
public:
	SphereMorphApplication() :
		GLApplication("Sphere to Torus Morphing", 800, 600, true, false),
		m_rotationController(800, 600),
		m_vertexCount(0),
		m_phase(0.0f)
	{
	}

	~SphereMorphApplication()
	{
		glDeleteProgram(m_program);
	}

protected:
	void OnInit() override
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		InitShaders();

		glLoadIdentity();
		gluLookAt(
			0.0, 5.0, 8.0, 
			0.0, 0.0, 0.0,  
			0.0, 1.0, 0.0   
		);

		int numStepsU = 40;
		int numStepsV = 40;
		float stepU = 2.0f * 3.14159265358979323846f / numStepsU;
		float stepV = 2.0f * 3.14159265358979323846f / numStepsV;

		std::vector<glm::fvec3> vertices;

		for (int i = 0; i < numStepsU; ++i)
		{
			for (int j = 0; j < numStepsV; ++j)
			{
				float u1 = i * stepU;
				float v1 = j * stepV;
				float u2 = (i + 1) * stepU;
				float v2 = (j + 1) * stepV;

				vertices.push_back(glm::fvec3(u1, v1, 0.0f));
				vertices.push_back(glm::fvec3(u2, v1, 0.0f));
				vertices.push_back(glm::fvec3(u2, v2, 0.0f));
				vertices.push_back(glm::fvec3(u1, v2, 0.0f));
			}
		}
		m_vertexCount = vertices.size();

		m_vertexBuffer.Create();
		m_vertexBuffer.BufferData(
			vertices.size() * sizeof(glm::fvec3),
			&vertices[0],
			GL_STATIC_DRAW
		);

		m_phaseLocation = m_program.GetUniformLocation("phase");
	}

	void OnDisplay() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_program);
		glUniform1f(m_phaseLocation, m_phase);

		m_vertexBuffer.Bind();
		glVertexPointer(3, GL_FLOAT, sizeof(glm::fvec3), m_vertexBuffer.GetBufferPointer());
		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_QUADS, 0, m_vertexCount);

		glDisableClientState(GL_VERTEX_ARRAY);
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
		gluPerspective(45.0, aspect, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void OnIdle() override
	{
		m_animationController.Tick();
		float delta = m_animationController.GetTimeDelta() * 0.001f;

		static float time = 0.0f;
		time += delta;
		m_phase = (sinf(time) + 1.0f) / 2.0f;

		PostRedisplay();
	}

	void InitShaders()
	{
		ShaderLoader loader;
		Shader vertexShader = loader.LoadShader(GL_VERTEX_SHADER, L"shaders/morph.vsh");

		m_program.Create();
		m_program.AttachShader(vertexShader);

		ShaderCompiler compiler;
		compiler.CompileShader(vertexShader);
		compiler.CheckStatus();

		ProgramLinker linker;
		linker.LinkProgram(m_program);
		linker.CheckStatus();
	}

private:
	Program m_program;
	size_t m_vertexCount;
	VertexBuffer m_vertexBuffer;
	RotationController m_rotationController;
	AnimationController m_animationController;
	GLint m_phaseLocation;
	float m_phase;
};