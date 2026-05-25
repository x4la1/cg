#pragma once
#include "pch.h"

class Smesharik
{
public:
    Smesharik()
        : m_position(0.0f, 0.0f)
        , m_scale(1.0f)
    {
    }

    void Draw() const
    {
        glPushMatrix();

        glTranslatef(m_position.x, m_position.y, 0.0f);
        glScalef(m_scale, m_scale, 1.0f);

        DrawBackParts();
        DrawBody();
        DrawFace();

        glPopMatrix();
    }

    void SetViewport(int width, int height, float left, float right, float bottom, float top)
    {
        m_width = std::max(width, 1);
        m_height = std::max(height, 1);

        m_left = left;
        m_right = right;
        m_bottom = bottom;
        m_top = top;
    }

    bool StartDragging(int x, int y)
    {
        glm::vec2 point = NormalizeCoords(x, y);

        if (!HitTest(point))
        {
            return false;
        }

        m_isDragging = true;
        m_dragOffset = m_position - point;

        return true;
    }

    void DragTo(int x, int y)
    {
        if (!m_isDragging)
        {
            return;
        }

        glm::vec2 point = NormalizeCoords(x, y);
        m_position = point + m_dragOffset;
    }

    void StopDragging()
    {
        m_isDragging = false;
    }

    bool HitTest(const glm::vec2& point) const
    {
        const float radius = 0.5f * m_scale;
        return glm::length(point - m_position) <= radius;
    }

    void SetPosition(const glm::vec2& position)
    {
        m_position = position;
    }

    glm::vec2 GetPosition() const
    {
        return m_position;
    }

    void SetScale(float scale)
    {
        m_scale = scale;
    }

    float GetScale() const
    {
        return m_scale;
    }

private:
    glm::vec2 m_position;
    float m_scale;

    bool m_isDragging = false;
    glm::vec2 m_dragOffset = glm::vec2(0.0f, 0.0f);

    int m_width = 1;
    int m_height = 1;

    float m_left = -1.0f;
    float m_right = 1.0f;
    float m_bottom = -1.0f;
    float m_top = 1.0f;

private:
    glm::vec2 NormalizeCoords(int x, int y) const
    {
        float nx = m_left + static_cast<float>(x) / static_cast<float>(m_width) * (m_right - m_left);

        float ny = m_top - static_cast<float>(y) / static_cast<float>(m_height) * (m_top - m_bottom);

        return glm::vec2(nx, ny);
    }

    void SetColor(float r, float g, float b) const
    {
        glColor3f(r, g, b);
    }

    void DrawCircle(float radius, int segments = 64) const
    {
        DrawEllipse(radius, radius, segments);
    }

    void DrawEllipse(float rx, float ry, int segments = 64) const
    {
        glBegin(GL_TRIANGLE_FAN);

        glVertex2f(0.0f, 0.0f);

        for (int i = 0; i <= segments; ++i)
        {
            const float angle =
                2.0f * 3.1415926f * static_cast<float>(i) / static_cast<float>(segments);

            glVertex2f(
                std::cos(angle) * rx,
                std::sin(angle) * ry
            );
        }

        glEnd();
    }

    void DrawEllipseOutline(float rx, float ry, int segments = 64) const
    {
        glBegin(GL_LINE_LOOP);

        for (int i = 0; i < segments; ++i)
        {
            const float angle =
                2.0f * 3.1415926f * static_cast<float>(i) / static_cast<float>(segments);

            glVertex2f(
                std::cos(angle) * rx,
                std::sin(angle) * ry
            );
        }

        glEnd();
    }

    void DrawFilledQuad(
        const glm::vec2& p1,
        const glm::vec2& p2,
        const glm::vec2& p3,
        const glm::vec2& p4) const
    {
        glBegin(GL_QUADS);

        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glVertex2f(p3.x, p3.y);
        glVertex2f(p4.x, p4.y);

        glEnd();
    }

    void DrawBackParts() const
    {
        // Уши
        SetColor(0.35f, 0.80f, 1.0f);

        glPushMatrix();
        glTranslatef(-0.12f, 0.32f, 0.0f);
        glRotatef(18.0f, 0.0f, 0.0f, 1.0f);
        DrawEllipse(0.07f, 0.25f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.12f, 0.32f, 0.0f);
        glRotatef(-18.0f, 0.0f, 0.0f, 1.0f);
        DrawEllipse(0.07f, 0.25f);
        glPopMatrix();

        // Внутренние части ушей
        SetColor(0.65f, 0.93f, 1.0f);

        glPushMatrix();
        glTranslatef(-0.12f, 0.33f, 0.0f);
        glRotatef(18.0f, 0.0f, 0.0f, 1.0f);
        DrawEllipse(0.03f, 0.16f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.12f, 0.33f, 0.0f);
        glRotatef(-18.0f, 0.0f, 0.0f, 1.0f);
        DrawEllipse(0.03f, 0.16f);
        glPopMatrix();

        // Ноги
        SetColor(0.20f, 0.62f, 0.95f);

        glPushMatrix();
        glTranslatef(-0.12f, -0.26f, 0.0f);
        glRotatef(-8.0f, 0.0f, 0.0f, 1.0f);
        DrawEllipse(0.10f, 0.05f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.12f, -0.26f, 0.0f);
        glRotatef(8.0f, 0.0f, 0.0f, 1.0f);
        DrawEllipse(0.10f, 0.05f);
        glPopMatrix();

        // Руки
        SetColor(0.25f, 0.70f, 0.98f);

        glPushMatrix();
        glTranslatef(-0.28f, -0.03f, 0.0f);
        glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
        DrawEllipse(0.05f, 0.14f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.28f, -0.03f, 0.0f);
        glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
        DrawEllipse(0.05f, 0.14f);
        glPopMatrix();
    }

    void DrawBody() const
    {
        SetColor(0.35f, 0.80f, 1.0f);
        DrawCircle(0.28f);

        SetColor(0.05f, 0.25f, 0.35f);

        glLineWidth(2.0f);
        DrawEllipseOutline(0.28f, 0.28f);
        glLineWidth(1.0f);
    }

    void DrawFace() const
    {
        // Глаза
        SetColor(1.0f, 1.0f, 1.0f);

        glPushMatrix();
        glTranslatef(-0.08f, 0.09f, 0.0f);
        DrawEllipse(0.08f, 0.10f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.08f, 0.09f, 0.0f);
        DrawEllipse(0.08f, 0.10f);
        glPopMatrix();

        // Контуры глаз
        SetColor(0.05f, 0.05f, 0.05f);

        glLineWidth(1.5f);

        glPushMatrix();
        glTranslatef(-0.08f, 0.09f, 0.0f);
        DrawEllipseOutline(0.08f, 0.10f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.08f, 0.09f, 0.0f);
        DrawEllipseOutline(0.08f, 0.10f);
        glPopMatrix();

        glLineWidth(1.0f);

        // Зрачки
        SetColor(0.0f, 0.0f, 0.0f);

        glPushMatrix();
        glTranslatef(-0.05f, 0.07f, 0.0f);
        DrawCircle(0.02f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.05f, 0.07f, 0.0f);
        DrawCircle(0.02f);
        glPopMatrix();

        // Нос
        SetColor(0.95f, 0.20f, 0.20f);

        glPushMatrix();
        glTranslatef(0.0f, 0.004f, 0.0f);
        DrawEllipse(0.04f, 0.03f);
        glPopMatrix();

        SetColor(0.35f, 0.05f, 0.05f);

        glLineWidth(1.2f);

        glPushMatrix();
        glTranslatef(0.0f, 0.004f, 0.0f);
        DrawEllipseOutline(0.04f, 0.03f);
        glPopMatrix();

        glLineWidth(1.0f);

        // Рот
        SetColor(0.35f, 0.05f, 0.08f);

        glBegin(GL_POLYGON);
        glVertex2f(-0.09f, -0.07f);
        glVertex2f(0.09f, -0.07f);
        glVertex2f(0.06f, -0.15f);
        glVertex2f(0.0f, -0.18f);
        glVertex2f(-0.06f, -0.15f);
        glEnd();

        // Зубы
        SetColor(1.0f, 1.0f, 1.0f);

        DrawFilledQuad(
            glm::vec2(-0.042f, -0.07f),
            glm::vec2(0.0f, -0.07f),
            glm::vec2(0.0f, -0.12f),
            glm::vec2(-0.042f, -0.12f)
        );

        DrawFilledQuad(
            glm::vec2(0.0f, -0.07f),
            glm::vec2(0.042f, -0.07f),
            glm::vec2(0.042f, -0.12f),
            glm::vec2(0.0f, -0.12f)
        );

        // Разделитель зубов
        SetColor(0.0f, 0.0f, 0.0f);

        glBegin(GL_LINES);
        glVertex2f(0.0f, -0.07f);
        glVertex2f(0.0f, -0.12f);
        glEnd();

        glLineWidth(1.0f);
    }
};