#pragma once
#include "pch.h"

class BaseTexture
{
public:
	void Create()
	{
		assert(!m_texture);
		glGenTextures(1, &m_texture);
	}

	void Delete()
	{
		assert(m_texture);
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}

	GLuint Detach()
	{
		GLuint texture = m_texture;
		m_texture = 0;
		return texture;
	}

	operator GLuint() const
	{
		return m_texture;
	}

	void BindTo(GLenum target) const
	{
		assert(m_texture != 0);
		glBindTexture(target, m_texture);
	}

protected:
	BaseTexture(GLuint texture)
		:m_texture(texture)
	{
	}

private:
	GLuint m_texture;

private:
	BaseTexture(BaseTexture const&);
	BaseTexture& operator=(BaseTexture const&);
};

template <bool t_managed, class TBase>
class TextureImpl : public TBase
{
public:
    TextureImpl(GLuint texture = 0)
        :TBase(texture)
    {
    }

    ~TextureImpl()
    {
        bool m = t_managed;
        if (m && (*this != 0))
        {
            TBase::Delete();
        }
    }

    // Присоединяем текстурный объект к экземпляру класса
    void Attach(GLuint texture)
    {
        if (t_managed && (*this != 0) && (texture != *this))
        {
            TBase::Delete();
        }
        TBase::SetTexture(texture);
    }
};

// Реализация двухмерного текстурного объекта
class Texture2DImpl : public BaseTexture
{
public:
    Texture2DImpl(GLuint texture = 0)
        :BaseTexture(texture) 
    {
    }

    void Bind() const
    {
        BindTo(GL_TEXTURE_2D);
    }

    static void TexImage(
        GLint level,
        GLint internalFormat,
        GLsizei width, GLsizei height,
        GLint border,
        GLenum format, GLenum type,
        const GLvoid* pixels)
    {
        glTexImage2D(GL_TEXTURE_2D, level, internalFormat,
            width, height, border, format, type, pixels);
    }
};

// Реализация одномерного текстурного объекта
class Texture1DImpl : public BaseTexture
{
public:
    Texture1DImpl(GLuint texture = 0)
        :BaseTexture(texture)
    {
    }

    virtual void Bind() const
    {
        BindTo(GL_TEXTURE_1D);
    }

    static void TexImage(
        GLint level,
        GLint internalFormat,
        GLsizei width,
        GLint border,
        GLenum format, GLenum type,
        const GLvoid* pixels)
    {
        glTexImage1D(GL_TEXTURE_1D, level, internalFormat,
            width, border, format, type, pixels);
    }
};

typedef TextureImpl<true, BaseTexture> Texture;
typedef TextureImpl<false, BaseTexture> TextureHandle;

typedef TextureImpl<true, Texture2DImpl> Texture2D;
typedef TextureImpl<false, Texture2DImpl> Texture2DHandle;

typedef TextureImpl<true, Texture1DImpl> Texture1D;
typedef TextureImpl<false, Texture1DImpl> Texture1DHandle;