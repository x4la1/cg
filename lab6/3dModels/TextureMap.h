#pragma once
#include "pch.h"
#include "BaseTexture.h"

class TextureMap
{
public:
	TextureMap(GLuint textureId = 0)
		:m_sx(1)
		, m_sy(1)
		, m_dx(0)
		, m_dy(0)
		, m_rotation(0)
		, m_texture(textureId)
	{

	}

	~TextureMap(void)
	{
	}

	void AttachTexture(GLuint textureId)
	{
		m_texture = textureId;
	}

	Texture2DHandle const& GetTexture()const
	{
		return m_texture;
	}

	void SetScale(float sx, float sy)
	{
		m_sx = sx;
		m_sy = sy;
	}

	void SetOffset(float dx, float dy)
	{
		m_dx = dx;
		m_dy = dy;
	}

	void SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	bool IsLoaded()const
	{
		return m_texture != 0;
	}

private:
	TextureMap(const TextureMap&);
	TextureMap& operator=(const TextureMap&);

	Texture2DHandle m_texture;
	float m_sx;
	float m_sy;
	float m_dx;
	float m_dy;
	float m_rotation;
};