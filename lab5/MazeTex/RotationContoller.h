#pragma once
#include "pch.h"

class RotationController
{
public:
	RotationController(int windowWidth, int windowHeight)
		: m_leftButtonPressed(false)
		, m_windowWidth(windowWidth)
		, m_windowHeight(windowHeight)
	{
	}

	void ResizeWindow(int windowWidth, int windowHeight)
	{
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
	}

	void OnMouse(int button, int state, int x, int y)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			m_leftButtonPressed = (state == GLUT_DOWN);
			m_mouseX = x;
			m_mouseY = y;
		}
	}

	void OnMotion(int x, int y)
	{
		if (m_leftButtonPressed)
		{
			int dx = x - m_mouseX;
			int dy = y - m_mouseY;

			GLfloat rotateX = GLfloat(dy) * 180 / m_windowHeight;
			GLfloat rotateY = GLfloat(dx) * 180 / m_windowWidth;

			RotateCamera(rotateX, rotateY);

			m_mouseX = x;
			m_mouseY = y;
		}
	}

	bool LeftButtonIsPressed() const
	{
		return m_leftButtonPressed;
	}

private:
	static void RotateCamera(GLfloat rotateX, GLfloat rotateY)
	{
		// »звлекаем текущее значение матрицы моделировани€-вида
		GLfloat modelView[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, &modelView[0]);

		// »звлекаем направлени€ координатных осей камеры в 3д пространстве
		// как коэффициенты строк матрицы моделировани€-вида
		glm::vec3 xAxis(modelView[0], modelView[4], modelView[8]);
		glm::vec3 yAxis(modelView[1], modelView[5], modelView[9]);

		// ѕоворачиваем вокруг осей x и y камеры
		glRotatef(rotateX, xAxis.x, xAxis.y, xAxis.z);
		glRotatef(rotateY, yAxis.x, yAxis.y, yAxis.z);

		// ¬ ходе умножени€ матриц могут возникать погрешности, которые,
		// накаплива€сь могут сильно искажать картинку
		// ƒл€ их компенсации после каждой модификации матрицы моделировани€-вида
		// проводим ее ортонормирование
		NormalizeModelViewMatrix();
	}

	static void NormalizeModelViewMatrix()
	{
		GLfloat modelView[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

		/*
		ќртонормирование - приведение координатных осей к единичной длине (нормирование)
		и взаимной перпендикул€рности (ортогонализаци€)
		ƒостичь этого можно при помощи нормализации координатных осей
		и векторного произведени€
		*/
		glm::vec3 xAxis(modelView[0], modelView[4], modelView[8]);
		xAxis = glm::normalize(xAxis);
		glm::vec3 yAxis(modelView[1], modelView[5], modelView[9]);
		yAxis = glm::normalize(yAxis);

		// ќсь Z вычисл€ем через векторное произведение X и Y
		// Z будет перпендикул€рна плоскости векторов X и Y
		glm::vec3 zAxis = glm::cross(xAxis, yAxis);
		// » иметь единичную длину
		zAxis = glm::normalize(zAxis);
		// “о же самое проделываем с ос€ми x и y
		xAxis = glm::cross(yAxis, zAxis);
		xAxis = glm::normalize(xAxis);
		yAxis = glm::cross(zAxis, xAxis);
		yAxis = glm::normalize(yAxis);

		modelView[0] = xAxis.x;
		modelView[4] = xAxis.y, modelView[8] = xAxis.z;
		modelView[1] = yAxis.x;
		modelView[5] = yAxis.y, modelView[9] = yAxis.z;
		modelView[2] = zAxis.x;
		modelView[6] = zAxis.y, modelView[10] = zAxis.z;

		glLoadMatrixf(modelView);
	}

	bool m_leftButtonPressed;
	int m_mouseX;
	int m_mouseY;
	int m_windowWidth;
	int m_windowHeight;
};