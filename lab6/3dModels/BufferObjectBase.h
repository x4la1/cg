#pragma once
#include "pch.h"

class BufferObjectBase
{
public:
	// Генерируем имя для буферного объекта
	void Create()
	{
		if (m_bufferId == 0)
		{
			glGenBuffers(1, &m_bufferId);
		}
	}

	// Возвращаем идентификатор буферного объекта
	operator GLuint() const
	{
		return m_bufferId;
	}

	// "Отсоединяем" буферный объект от текущего экземпляра класса
	GLuint Detach()
	{
		GLuint bufferId = m_bufferId;
		m_bufferId = 0;
		return bufferId;
	}

	// Удаляем буферный объект
	void Delete()
	{
		if (m_bufferId != 0)
		{
			glDeleteBuffers(1, &m_bufferId);
			m_bufferId = 0;
		}
	}
protected:
	BufferObjectBase(GLuint bufferId)
		: m_bufferId(bufferId)
	{
	}

	void BindTo(GLenum target) const
	{
		glBindBuffer(target, m_bufferId);
	}

	void SetBuffer(GLuint bufferId)
	{
		m_bufferId = bufferId;
	}

private:
	GLuint m_bufferId;

	BufferObjectBase(BufferObjectBase const&);
	BufferObjectBase& operator=(BufferObjectBase const&);
};


template <bool t_managed, GLenum target>
class BufferObjectImpl : public BufferObjectBase
{
public:
	BufferObjectImpl<t_managed, target>(GLuint bufferId = 0)
		: BufferObjectBase(bufferId)
	{
	}

	~BufferObjectImpl()
	{
		if (t_managed && (*this != 0))
		{
			Delete();
		}
	}

	void Bind() const
	{
		BindTo(target);
	}

	void Attach(GLuint bufferId)
	{
		if (t_managed && (*this != 0) && (bufferId != *this))
		{
			Delete();
		}
		SetBuffer(bufferId);
	}

	static GLvoid* MapBuffer(GLenum access)
	{
		return glMapBuffer(target, access);
	}

	static GLboolean UnmapBuffer()
	{
		return glUnmapBuffer(target);
	}

	static void BufferData(GLsizeiptr size, GLvoid const* data, GLenum usage)
	{
		glBufferData(target, size, data, usage);
	}

	static void BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data)
	{
		glBufferSubData(target, offset, size, data);
	}

	BufferObjectImpl<t_managed, target>& operator=(GLuint bufferId)
	{
		Attach(bufferId);
		return *this;
	}
};

typedef BufferObjectImpl<true, GL_ARRAY_BUFFER> ArrayBuffer;
typedef BufferObjectImpl<false, GL_ARRAY_BUFFER> ArrayBufferHandle;
typedef BufferObjectImpl<true, GL_ELEMENT_ARRAY_BUFFER> ElementArrayBuffer;
typedef BufferObjectImpl<false, GL_ELEMENT_ARRAY_BUFFER> ElementArrayBufferHandle;