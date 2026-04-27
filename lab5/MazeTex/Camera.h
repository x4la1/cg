#pragma once
#include "pch.h"

class Camera
{
public:
	Camera()
	{
		m_transform = glm::dmat4(1.0);
	}
	virtual ~Camera() {}

	void LookAt(double eyeX, double eyeY, double eyeZ,
		double lookAtX, double lookAtY, double lookAtZ,
		double upX, double upY, double upZ)
	{
		m_transform = glm::lookAt(
			glm::dvec3(eyeX, eyeY, eyeZ),
			glm::dvec3(lookAtX, lookAtY, lookAtZ),
			glm::dvec3(upX, upY, upZ)
		);
	}

	void LoadToOpenGL() const
	{
		glLoadMatrixd(glm::value_ptr(m_transform));
	}

	void Normalize()
	{
		glm::dvec3 xAxis(m_transform[0][0], m_transform[1][0], m_transform[2][0]);
		glm::dvec3 yAxis(m_transform[0][1], m_transform[1][1], m_transform[2][1]);

		xAxis = glm::normalize(xAxis);
		yAxis = glm::normalize(yAxis);

		glm::dvec3 zAxis = glm::normalize(glm::cross(xAxis, yAxis));
		yAxis = glm::normalize(glm::cross(zAxis, xAxis));

		m_transform[0][0] = xAxis.x; m_transform[1][0] = xAxis.y; m_transform[2][0] = xAxis.z;
		m_transform[0][1] = yAxis.x; m_transform[1][1] = yAxis.y; m_transform[2][1] = yAxis.z;
		m_transform[0][2] = zAxis.x; m_transform[1][2] = zAxis.y; m_transform[2][2] = zAxis.z;
	}

	void Move(double dx, double dy, double dz)
	{
		glm::dmat4 mat = glm::translate(glm::dmat4(1.0), glm::dvec3(dx, dy, dz));
		m_transform = mat * m_transform;
	}

	void Roll(double angleInRadians)
	{
		glm::dmat4 mat = glm::rotate(glm::dmat4(1.0), angleInRadians, glm::dvec3(0.0, 0.0, 1.0));
		m_transform = mat * m_transform;
	}

	void Yaw(double angleInRadians)
	{
		glm::dmat4 mat = glm::rotate(glm::dmat4(1.0), angleInRadians, glm::dvec3(0.0, 1.0, 0.0));
		m_transform = mat * m_transform;
	}

private:
	glm::dmat4 m_transform;
};