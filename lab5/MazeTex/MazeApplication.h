#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "Camera.h"
#include "DirectLight.h"
#include "TextureLoader.h"
#include "TexturedMaze.h"

class MazeApplication : public GLApplication
{
public:
	MazeApplication() : GLApplication("3D Maze - OpenGL", 800, 600, true, false)
	{
		m_cameraX = 1.5;
		m_cameraZ = 1.5;
		m_cameraYaw = 0.0;
	}

protected:
	void OnInit() override
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);

		m_light.SetDirection({ -0.5f, -1.0f, -0.5f });
		m_light.SetAmbientIntensity(0.4f, 0.4f, 0.4f);
		m_light.SetDiffuseIntensity(0.7f, 0.7f, 0.7f);
		m_light.SetLight(GL_LIGHT0);

		TextureLoader loader;
		m_maze.LoadTextures(loader);
	}

	void OnDisplay() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		double lookX = m_cameraX + cos(m_cameraYaw);
		double lookZ = m_cameraZ + sin(m_cameraYaw);

		m_camera.LookAt(
			m_cameraX, 0.5, m_cameraZ,
			lookX, 0.5, lookZ,
			0.0, 1.0, 0.0
		);
		m_camera.LoadToOpenGL();

		m_maze.Draw();
	}

	void OnKeyboard(unsigned char key, int x, int y) override
	{
		double nextX = m_cameraX;
		double nextZ = m_cameraZ;

		switch (key)
		{
		case 'w': case 'W':
			nextX += cos(m_cameraYaw) * CAMERA_MOVE_SPEED;
			nextZ += sin(m_cameraYaw) * CAMERA_MOVE_SPEED;
			break;
		case 's': case 'S':
			nextX -= cos(m_cameraYaw) * CAMERA_MOVE_SPEED;
			nextZ -= sin(m_cameraYaw) * CAMERA_MOVE_SPEED;
			break;
		case 'a': case 'A':
			m_cameraYaw -= CAMERA_ROTATION_SPEED;
			break;
		case 'd': case 'D':
			m_cameraYaw += CAMERA_ROTATION_SPEED;
			break;
		case 27:
			exit(0);
			break;
		}

		if (m_maze.CanMove(nextX, m_cameraZ, CAMERA_RADIUS))
		{
			m_cameraX = nextX;
		}

		if (m_maze.CanMove(m_cameraX, nextZ, CAMERA_RADIUS))
		{
			m_cameraZ = nextZ;
		}

		PostRedisplay();
	}

	void OnReshape(int width, int height) override
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, (GLdouble)width / (GLdouble)height, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
	}

private:
	Camera m_camera;
	DirectLight m_light;
	TexturedMaze m_maze;

	double m_cameraX;
	double m_cameraZ;
	double m_cameraYaw;

	static constexpr double CAMERA_RADIUS = 0.2;
	static constexpr double CAMERA_MOVE_SPEED = 0.15;
	static constexpr double CAMERA_ROTATION_SPEED = 0.1;
};