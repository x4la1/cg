#pragma once
#include "pch.h"

class ShaderBase
{
protected:
	ShaderBase(GLuint shader = 0)
		:m_shader(shader)
	{
	}
	~ShaderBase()
	{
	}
private:
	ShaderBase(ShaderBase const&);
	ShaderBase& operator=(ShaderBase const&);

	GLuint m_shader;

public:
	void SetSource(GLsizei count, const GLchar** strings, const GLint* lengths)
	{
		assert(m_shader != 0);

		glShaderSource(m_shader, count, strings, lengths);
	}

	// Задаем исходный код шейдера (в одной строке)
	void SetSource(const GLchar* source, GLint length)
	{
		const GLchar** ppSource = &source;
		SetSource(1, ppSource, &length);
	}

	// Задаем исходный код шейдера (одна ASCIIZ-строка)
	void SetSource(const GLchar* source)
	{
		const GLchar** ppSource = &source;
		SetSource(1, ppSource, NULL);
	}

	// Получаем значение некоторого параметра шейдера
	void GetParameter(GLenum pname, GLint* param)const
	{
		assert(m_shader != 0);
		glGetShaderiv(m_shader, pname, param);
	}

	// Упрощенный вариант предыдущего метода
	GLint GetParameter(GLenum pname)const
	{
		GLint value;
		GetParameter(pname, &value);
		return value;
	}

	// Получаем информационный лог от шейдера
	void GetInfoLog(GLsizei bufSize, GLsizei* length, GLchar* infoLog)const
	{
		assert(m_shader != 0);
		glGetShaderInfoLog(m_shader, bufSize, length, infoLog);
	}

	// Получаем информационный лог от шейдера
	// (упрощенный вариант)
	std::string GetInfoLog()const
	{
		GLint length = GetParameter(GL_INFO_LOG_LENGTH);
		if (length > 0)
		{
			std::vector<char> buffer(length);
			GetInfoLog(length, &length, &buffer[0]);
			std::string log(&buffer[0]);
			return log;
		}
		else
		{
			return std::string();
		}
	}


	// Получаем идентификатор шейдерного объекта
	operator GLuint()const
	{
		return m_shader;
	}

	// Получаем идентификатор шейдерного объекта (альтернативный вариант)
	GLuint Get()const
	{
		return m_shader;
	}

	// Компилируем ешйдер
	void Compile()
	{
		assert(m_shader != 0);
		glCompileShader(m_shader);
	}

	// Присоединяем шейдерный объект к классу и возвращаем
	// дескриптор ранее присоединенного
	GLuint Attach(GLuint shader)
	{
		GLuint tmp = m_shader;
		m_shader = shader;
		return tmp;
	}

	// Отсоединяем шейдерный объект
	GLuint Detach()
	{
		return Attach(0);
	}

	// Удаляем шейдерный объект
	void Delete()
	{
		assert(m_shader != 0);
		if (m_shader != 0)
		{
			glDeleteShader(Detach());
			m_shader = 0;
		}
	}
};

template <bool t_managed>
class ShaderImpl : public ShaderBase
{
public:
	ShaderImpl(GLuint shader = 0)
		:ShaderBase(shader)
	{
	}

	// Оператор присваивания выполняет
	ShaderImpl& operator=(GLuint shader)
	{
		// Удаляем текущий шейдерный объект только если:
		if (
			t_managed && // Класс управляет шейдерным объектом
			(Get() != 0) && // Задан текущий шейдерный объект
			(Get() != shader)    // И он не совпадает с переданным
			)
		{
			Delete();
		}
		Attach(shader);
		return *this;
	}

	// Создаем шейдер заданного типа
	GLuint Create(GLenum type)
	{
		// Удаляем старый шейдерный объект в случае необходимости
		// (если он задан и класс управляет шейдером)
		if ((Get() != 0) && t_managed)
		{
			Delete();
		}
		GLuint shader = glCreateShader(type);
		Attach(shader);
		return shader;
	}

	// Деструктор удалит текущий шейдерный объект (если класс управляет
	// шейдерным объектом)
	~ShaderImpl(void)
	{
		if (t_managed && (Get() != 0))
		{
			Delete();
		}
	}
};

typedef ShaderImpl<true > Shader;
typedef ShaderImpl<false> ShaderHandle;