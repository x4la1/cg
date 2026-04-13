#pragma once
#include "pch.h"

class Material
{
public:
	Material()
	{
        SetDiffuse(1, 1, 1);
        SetAmbient(0.2f, 0.2f, 0.2f);
        SetSpecular(1, 1, 1);
        SetShininess(0);
	}

    void SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1)
    {
        m_diffuse[0] = r;
        m_diffuse[1] = g;
        m_diffuse[2] = b;
        m_diffuse[3] = a;
    }

    // Коэффициент фонового отражения
    void SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1)
    {
        m_ambient[0] = r;
        m_ambient[1] = g;
        m_ambient[2] = b;
        m_ambient[3] = a;
    }

    // Коэффициент зеркального отражения
    void SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1)
    {
        m_specular[0] = r;
        m_specular[1] = g;
        m_specular[2] = b;
        m_specular[3] = a;
    }

    // Степень зеркального отражения (для модели Фонга)
    void SetShininess(GLfloat shininess)
    {
        m_shininess = shininess;
    }

    void Activate(GLenum side = GL_FRONT) const
    {
        glMaterialfv(side, GL_DIFFUSE, m_diffuse);
        glMaterialfv(side, GL_AMBIENT, m_ambient);
        glMaterialfv(side, GL_SPECULAR, m_specular);
        glMaterialf(side, GL_SHININESS, m_shininess);
    }

private:
    GLfloat m_shininess;
    GLfloat m_diffuse[4];
    GLfloat m_specular[4];
    GLfloat m_ambient[4];
};