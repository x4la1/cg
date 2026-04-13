#pragma once
#include "pch.h"
#include "BaseTexture.h"
#include "TextureLoader.h"

class TexturedMaze
{
public:
    TexturedMaze()
    {
        m_map = 
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {3, 0, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 0, 3},
            {4, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 4},
            {5, 0, 6, 0, 1, 1, 1, 1, 1, 1, 1, 0, 6, 0, 5},
            {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
            {1, 0, 2, 0, 3, 0, 3, 3, 3, 0, 3, 0, 2, 0, 1},
            {2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 2},
            {3, 0, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 0, 3},
            {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}
        };

        m_mapWidth = m_map[0].size();
        m_mapHeight = m_map.size();

        for (int i = 0; i < 6; ++i)
        {
            m_wallTextures[i] = 0;
        }

        m_floorTexture = 0;
    }

    void LoadTextures(const TextureLoader& loader)
    {
        m_wallTextures[0] = loader.LoadTexture2D(L"tex1.jpg");
        m_wallTextures[1] = loader.LoadTexture2D(L"tex1.jpg");
        m_wallTextures[2] = loader.LoadTexture2D(L"tex1.jpg");
        m_wallTextures[3] = loader.LoadTexture2D(L"tex1.jpg");
        m_wallTextures[4] = loader.LoadTexture2D(L"tex1.jpg");
        m_wallTextures[5] = loader.LoadTexture2D(L"tex1.jpg");

        m_floorTexture = loader.LoadTexture2D(L"tex2.png");
    }

    void Draw() const
    {
        DrawFloor();

        for (int z = 0; z < m_mapHeight; ++z)
        {
            for (int x = 0; x < m_mapWidth; ++x)
            {
                int cell = m_map[z][x];

                if (cell >= 1 && cell <= 6)
                {
                    int texIndex = cell - 1;

                    glPushMatrix();
                    glTranslated(x + 0.5, 0.0, z + 0.5);
                    Texture2DHandle(m_wallTextures[texIndex]).Bind();
                    DrawCube();
                    glPopMatrix();
                }
            }
        }
    }

    bool CanMove(double x, double z, double radius = 0.2) const
    {
        if (IsWall(x - radius, z - radius) || 
            IsWall(x + radius, z - radius) || 
            IsWall(x - radius, z + radius) || 
            IsWall(x + radius, z + radius))
        {
            return false;
        }

        return true;
    }

    bool IsWall(double x, double z) const
    {
        int gridX = (int)x;
        int gridZ = (int)z;

        if (gridX < 0 || gridX >= m_mapWidth || gridZ < 0 || gridZ >= m_mapHeight)
        {
            return true;
        }

        return m_map[gridZ][gridX] != 0;
    }

private:
    void DrawCube() const
    {
        glBegin(GL_QUADS);
        
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.0f, 0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 1.0f, 0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 1.0f, 0.5f);
        
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.0f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 1.0f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 1.0f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.0f, -0.5f);
        
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 1.0f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 1.0f, 0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 1.0f, 0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 1.0f, -0.5f);
        
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.0f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.0f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.0f, 0.5f);
        
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.0f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 1.0f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 1.0f, 0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.5f);
        
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.0f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.0f, 0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 1.0f, 0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 1.0f, -0.5f);
        glEnd();
    }

    void DrawFloor() const
    {
        Texture2DHandle(m_floorTexture).Bind();
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);

        glTexCoord2f(0.0f, 0.0f); 
        glVertex3f(0.0f, 0.0f, m_mapHeight);

        glTexCoord2f(m_mapWidth, 0.0f); 
        glVertex3f(m_mapWidth, 0.0f, m_mapHeight);

        glTexCoord2f(m_mapWidth, m_mapHeight);
        glVertex3f(m_mapWidth, 0.0f, 0.0f);

        glTexCoord2f(0.0f, m_mapHeight);
        glVertex3f(0.0f, 0.0f, 0.0f);

        glEnd();
    }

    std::vector<std::vector<int>> m_map;
    int m_mapWidth;
    int m_mapHeight;

    GLuint m_wallTextures[6];
    GLuint m_floorTexture;
};
