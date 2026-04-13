#pragma once
#include "pch.h"

struct Face
{
	std::vector<int> indices;
	glm::vec3 normal;
	glm::vec4 color;
};

class DeltoidalHexecontahedron
{
public:
	DeltoidalHexecontahedron(float radius)
	{
		srand(static_cast<unsigned>(time(0)));
		Generate(radius);
	}

	void Draw()
	{
		for (const auto& face : m_faces)
		{
			glBegin(GL_QUADS);
			glNormal3fv(glm::value_ptr(face.normal));
			glColor4fv(glm::value_ptr(face.color));
			for (int idx : face.indices) 
			{
				glVertex3fv(glm::value_ptr(m_vertices[idx]));
			}
			glEnd();
		}
	}

	void DrawLines()
	{
		glLineWidth(2.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		for (const auto& face : m_faces)
		{
			glBegin(GL_LINE_LOOP);
			for (int idx : face.indices) 
			{
				glVertex3fv(glm::value_ptr(m_vertices[idx]));
			}
			glEnd();
		}
	}

private:
	void Generate(float radius)
	{
		float phi = (1 + sqrt(5)) / 2;
		glm::vec3 ico[12] = 
		{
			{0, 1, phi}, {0, -1, phi}, {0, 1, -phi}, {0, -1, -phi},
			{1, phi, 0}, {-1, phi, 0}, {1, -phi, 0}, {-1, -phi, 0},
			{phi, 0, 1}, {-phi, 0, 1}, {phi, 0, -1}, {-phi, 0, -1}
		};

		float scale5 = sqrt((9.0f + 4.0f * sqrt(5.0f)) / (9.0f + 3.6f * sqrt(5.0f)));
		float scale3 = sqrt((9.0f + 4.0f * sqrt(5.0f)) / (29.0f / 3.0f + 4.0f * sqrt(5.0f)));

		std::cout << scale5 << "\n" << scale3;

		glm::vec3 v3, p5I, p5J, p5K, p4IJ, p4JK, p4KI;

		for (int i = 0; i < 12; ++i) 
		{
			for (int j = i + 1; j < 12; ++j) 
			{
				for (int k = j + 1; k < 12; ++k) 
				{
					if (abs(glm::distance(ico[i], ico[j]) - 2.0f) < 0.1f &&
						abs(glm::distance(ico[j], ico[k]) - 2.0f) < 0.1f &&
						abs(glm::distance(ico[k], ico[i]) - 2.0f) < 0.1f) 
					{

						v3 = glm::normalize(ico[i] + ico[j] + ico[k]) * scale3 * radius;
						p5I = glm::normalize(ico[i]) * scale5 * radius;
						p5J = glm::normalize(ico[j]) * scale5 * radius;
						p5K = glm::normalize(ico[k]) * scale5 * radius;
						p4IJ = glm::normalize(ico[i] + ico[j]) * radius;
						p4JK = glm::normalize(ico[j] + ico[k]) * radius;
						p4KI = glm::normalize(ico[k] + ico[i]) * radius;

						AddFace(v3, p4IJ, p5J, p4JK);
						AddFace(v3, p4JK, p5K, p4KI);
						AddFace(v3, p4KI, p5I, p4IJ);
					}
				}
			}
		}
	}



	void AddFace(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
	{
		Face f;
		glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
		glm::vec3 center = (a + b + c + d) / 4.0f;

		if (glm::dot(normal, center) < 0)
		{
			normal = -normal;
			f.indices = { AddVertex(a), AddVertex(d), AddVertex(c), AddVertex(b) };
		}
		else
		{
			f.indices = { AddVertex(a), AddVertex(b), AddVertex(c), AddVertex(d) };
		}

		f.normal = normal;

		f.color.r = static_cast<float>(rand()) / RAND_MAX;
		f.color.g = static_cast<float>(rand()) / RAND_MAX;
		f.color.b = static_cast<float>(rand()) / RAND_MAX;
		f.color.a = 0.5f;

		m_faces.push_back(f);
	}

	int AddVertex(glm::vec3 v)
	{
		for (int i = 0; i < m_vertices.size(); ++i)
		{
			if (glm::distance(m_vertices[i], v) < 0.0001f)
			{
				return i;
			}
		}
		m_vertices.push_back(v);
		return m_vertices.size() - 1;
	}

private:
	std::vector<Face> m_faces;
	std::vector<glm::vec3> m_vertices;
};
