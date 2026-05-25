#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "BezierCurve.h"

class MyApplication : public GLApplication
{
public:
	MyApplication() :
		GLApplication("Bezier", 800, 600, true, false)
	{
	}

	~MyApplication()
	{
	}

protected:
	void OnInit() override
	{
		glClearColor(1, 1, 1, 1);

		glDisable(GL_DEPTH_TEST);
	}

	void OnDisplay() override
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_curve.Draw();
	}

	void OnReshape(int width, int height) override
	{
		glViewport(0, 0, width, height);

		float aspect = static_cast<float>(width) / static_cast<float>(height);

		float left = -1.0f;
		float right = 1.0f;
		float bottom = -1.0f;
		float top = 1.0f;

		if (aspect > 1.0f)
		{
			left = -aspect;
			right = aspect;
		}
		else
		{
			bottom = -1.0f / aspect;
			top = 1.0f / aspect;
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(left, right, bottom, top);

		glMatrixMode(GL_MODELVIEW);

		m_curve.SetViewportSize(width, height, left, right, bottom, top);
	}
	
	void OnMouse(int button, int state, int x, int y) override
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				m_curve.StartDragging(x, y);
			}
			else if (state == GLUT_UP)
			{
				m_curve.StopDragging();
			}

			PostRedisplay();
		}
	}

	void OnMotion(int x, int y) override
	{
		m_curve.DragTo(x, y);
		PostRedisplay();
	}

private:
	BezierCurve m_curve;
};