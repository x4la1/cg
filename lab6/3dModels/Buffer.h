#pragma once
#include "pch.h"
#include "BufferObjectBase.h"

template <GLenum target>
class BufferImpl
{
public:
	BufferImpl(bool useVBO = true)
		:m_hasData(false)
		, m_useVBO(useVBO)
		, m_created(false)
	{
	}

	void Create()
	{
		if (!m_created)
		{
			m_useVBO = m_useVBO && (GLEW_ARB_vertex_buffer_object == GL_TRUE);

			if (m_useVBO && (m_buffer == 0))
			{
				m_buffer.Create();
			}
			m_created = true;
		}
	}

	void Bind() const
	{
		if (!m_created)
		{
			throw std::logic_error("The buffer has not been created");
		}

		if (m_useVBO)
		{
			m_buffer.Bind();
		}
		else if (GLEW_ARB_vertex_buffer_object)
		{
			glBindBuffer(target, 0);
		}

		UpdateCurrentBufferPointer(false, this);
	}

	static void Unbind()
	{
		if (GLEW_ARB_vertex_buffer_object)
		{
			glBindBuffer(target, 0);
		}

		UpdateCurrentBufferPointer(false, NULL);
	}

	void BufferData(GLsizeiptr size, GLvoid const* data, GLenum usage)
	{
		UpdateCurrentBufferPointer(true, this);

		if (m_useVBO)
		{
			m_buffer.BufferData(size, data, usage);
		}
		else
		{
			if (data != NULL)
			{
				GLubyte const* pDataStart = reinterpret_cast<GLubyte const*>(data);
				GLubyte const* pDataEnd = pDataStart + size;
				m_data.assign(pDataStart, pDataEnd);
			}
			else
			{
				m_data.clear();
			}
		}

		m_hasData = (data != NULL);
	}

	GLvoid const* GetBufferPointer()const
	{
		if (!m_hasData)
		{
			throw std::logic_error("The buffer contains no data");
		}

		UpdateCurrentBufferPointer(true, this);

		if (m_useVBO)
		{
			return NULL;
		}
		else
		{
			return &m_data[0];
		}
	}

	~BufferImpl()
	{
		UpdateCurrentBufferPointer(false, NULL);
	}

private:
	static void UpdateCurrentBufferPointer(bool callBind, BufferImpl const* thisPtr)
	{
		static const BufferImpl* pCurrentBuffer = NULL;

		if (pCurrentBuffer != thisPtr)
		{
			pCurrentBuffer = thisPtr;

			if (callBind && (thisPtr != NULL))
			{
				thisPtr->Bind();
			}
		}
	}


private:
	BufferImpl(BufferImpl const&);
	BufferImpl& operator=(BufferImpl const&);

	BufferObjectImpl<true, target> m_buffer;
	std::vector<BYTE>	m_data;

	bool m_hasData;	
	bool m_useVBO;	
	bool m_created; 
};

typedef BufferImpl<GL_ARRAY_BUFFER> VertexBuffer;
typedef BufferImpl<GL_ELEMENT_ARRAY_BUFFER> IndexBuffer;