#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "Smesharik.h"

class MyApplication : public GLApplication
{
public:
    MyApplication()
        : GLApplication("Smeshariki Character", 800, 600, false, false)
    {
    }

    ~MyApplication()
    {
    }

protected:
    void OnInit() override
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OnDisplay() override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        m_smesharik.Draw();
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

        m_smesharik.SetViewport(width, height, left, right, bottom, top);
    }

    void OnMouse(int button, int state, int x, int y) override
    {
        if (button != GLUT_LEFT_BUTTON)
        {
            return;
        }

        if (state == GLUT_DOWN)
        {
            m_smesharik.StartDragging(x, y);
        }
        else if (state == GLUT_UP)
        {
            m_smesharik.StopDragging();
        }

        PostRedisplay();
    }

    void OnMotion(int x, int y) override
    {
        m_smesharik.DragTo(x, y);

        PostRedisplay();
    }

private:
    Smesharik m_smesharik;
};