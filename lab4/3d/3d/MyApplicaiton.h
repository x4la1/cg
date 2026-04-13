#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "RotationContoller.h"
#include "DeltoidalHexecontahedron.h"

class MyApplication : public GLApplication
{
public:
	MyApplication(const char* title, int width, int height)
		: GLApplication(title, width, height),
		m_rotationController(width, height)
	{
	}

	~MyApplication(void) {}

protected:
	virtual void OnInit()
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);

		gluLookAt
		(
			0, 0, 12,
			0, 0, 0,
			0, 1, 0
		);
	}

	virtual void OnDisplay()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);


		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);

		m_polyhedron.Draw();

		glDisable(GL_POLYGON_OFFSET_FILL);

		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);

		m_polyhedron.DrawLines();
	}

	virtual void OnReshape(int width, int height)
	{
		glViewport(0, 0, width, height);

		m_rotationController.ResizeWindow(width, height);

		double aspect = double(width) / double(height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(m_fov, aspect, m_zNear, m_zFar);
		glMatrixMode(GL_MODELVIEW);
	}

	virtual void OnMouse(int button, int state, int x, int y)
	{
		m_rotationController.OnMouse(button, state, x, y);
	}

	virtual void OnMotion(int x, int y)
	{
		if (m_rotationController.LeftButtonIsPressed())
		{
			m_rotationController.OnMotion(x, y);
			PostRedisplay();
		}
	}

private:
	RotationController m_rotationController;
	DeltoidalHexecontahedron m_polyhedron{ 1.5f };
	double m_fov = 45;
	double m_zNear = 0.1;
	double m_zFar = 15;
};
