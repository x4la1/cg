#include "pch.h"
#include "ProgramBase.h"
#include "Buffer.h"

struct Vertex
{
	glm::fvec3 startPos;
	glm::fvec3 endPos;
};

class MorphShape
{
public:
	MorphShape(size_t numberOfVertices, float maxRadius, float minRadius)
		:m_pStartPosArray(NULL)
		, m_pEndPosArray(NULL)
		, m_numberOfVertices(numberOfVertices)
		, m_maxRadius(maxRadius)
		, m_minRadius(minRadius)
		, m_initialized(false)
	{

	}

	void SetProgram(
		GLuint program, GLuint phaseLocation, GLuint endPosLocation)
	{
		m_program = program;
		m_phaseLocation = phaseLocation;
		m_endPosLocation = endPosLocation;
	}

	void Draw(float phase)const
	{
		if (!m_program)
		{
			throw std::logic_error("Shader program was not specified");
		}

		if (!m_initialized)
		{
			Create();
		}

		// Подключаем шейдерную программу
		glUseProgram(m_program);

		// Инициализируем uniform-переменные
		glUniform1f(m_phaseLocation, phase);

		// Выполняем привязку вершинного буфера
		m_vertexBuffer.Bind();

		// Инициализируем адреса массивов вершин
		glVertexPointer(
			3, GL_FLOAT, sizeof(Vertex), m_pStartPosArray);

		glVertexAttribPointer(
			m_endPosLocation, 3, GL_FLOAT,
			GL_FALSE, sizeof(Vertex), m_pEndPosArray);

		// Разрешаем использование массивов вершин и атрибутов
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableVertexAttribArray(m_endPosLocation);

		// Рисуем объект
		glDrawArrays(GL_LINE_LOOP, 0, m_numberOfVertices);

		// Запрещаем использование массивов вершин и атрибутов
		glDisableVertexAttribArray(m_endPosLocation);
		glDisableClientState(GL_VERTEX_ARRAY);

		// Отключаем программу
		glUseProgram(0);
	}

private:
	MorphShape(MorphShape const&);
	MorphShape& operator=(MorphShape const&);

	void Create()const
	{
		// Подготавливаем массив с информацией о вершинах
		std::vector<Vertex> vertices(m_numberOfVertices);
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			Vertex& v = vertices[i];
			// Вычисляем начальные координаты вершины
			float angle = static_cast<float>(2 * 3.14159265358979323846 * i / m_numberOfVertices);
			float ca = cosf(angle);
			float sa = sinf(angle);

			v.startPos =
				glm::fvec3(m_minRadius * ca, m_minRadius * sa, 0);

			// Вычисляем конечные координаты вершины
			float r2 = m_minRadius +
				(m_maxRadius - m_minRadius) * (cosf(6 * angle) + 1) / 2;
			v.endPos =
				glm::fvec3(r2 * ca, r2 * sa, 0);
		}
		// Создаем и заполняем данными буфер вершин
		m_vertexBuffer.Create();
		m_vertexBuffer.BufferData(
			sizeof(Vertex) * m_numberOfVertices,
			&vertices[0],
			GL_STATIC_DRAW
		);

		// Запоминаем адреса массивов начальных и конечных координат вершин
		const GLubyte* pBufferPointer =
			static_cast<const GLubyte*>(m_vertexBuffer.GetBufferPointer());

		m_pStartPosArray = pBufferPointer + offsetof(Vertex, startPos);
		m_pEndPosArray = pBufferPointer + offsetof(Vertex, endPos);

		// Отмечаем, что массив был проинициализирован
		m_initialized = true;
	}

	// Количество вершин в фигуре
	size_t const m_numberOfVertices;

	// Минимальный и максимальный радиус лепестков
	float const m_maxRadius;
	float const m_minRadius;

	// Вершинный буфер и указатели на находящиеся в нем массивы
	mutable bool m_initialized;
	mutable VertexBuffer m_vertexBuffer;
	mutable GLubyte const* m_pStartPosArray;
	mutable GLubyte const* m_pEndPosArray;

	ProgramHandle m_program;
	GLuint m_phaseLocation;
	GLuint m_endPosLocation;
};