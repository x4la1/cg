#pragma once
#include "pch.h"

class DirectLight
{
public:
	DirectLight(const glm::vec3& direction = { 0, 0, 1 })
		:m_direction(direction)
	{
		SetDiffuseIntensity(0.8, 0.8, 0.8, 1);
		SetAmbientIntensity(0.2, 0.2, 0.2, 1);
		SetSpecularIntensity(0.5, 0.5, 0.5, 1);
	}

	void SetDirection(const glm::vec3& direction)
	{
		m_direction = direction;
	}

	void SetDiffuseIntensity(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1)
	{
		m_diffuse[0] = r;
		m_diffuse[1] = g;
		m_diffuse[2] = b;
		m_diffuse[3] = a;
	}

	void SetAmbientIntensity(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1)
	{
		m_ambient[0] = r;
		m_ambient[1] = g;
		m_ambient[2] = b;
		m_ambient[3] = a;
	}

	void SetSpecularIntensity(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1)
	{
		m_specular[0] = r;
		m_specular[1] = g;
		m_specular[2] = b;
		m_specular[3] = a;
	}

	void SetLight(GLenum light)
	{
		GLfloat lightDirection[4] =
		{
			m_direction.x,
			m_direction.y,
			m_direction.z,
			0
		};
		glLightfv(light, GL_POSITION, lightDirection);
		glLightfv(light, GL_DIFFUSE, m_diffuse);
		glLightfv(light, GL_AMBIENT, m_ambient);
		glLightfv(light, GL_SPECULAR, m_specular);
	}

private:
	glm::vec3 m_direction;
	GLfloat m_diffuse[4];
	GLfloat m_ambient[4];
	GLfloat m_specular[4];
};
