#pragma once
#include "pch.h"
#include "TextureMap.h"
#include "Material.h"

class ModelMaterial
{
public:
	ModelMaterial()
		:m_twoSided(false)
	{
	}

	TextureMap& AddTextureMap1(GLuint texture)
	{
		m_textureMap1.AttachTexture(texture);
		return m_textureMap1;
	}

	bool HasTextureMap1()const
	{
		return m_textureMap1.IsLoaded();
	}

	TextureMap const& GetTextureMap1()const
	{
		return m_textureMap1;
	}

	TextureMap& GetTextureMap1()
	{
		return m_textureMap1;
	}

	Material& GetMaterial()
	{
		return m_material;
	}

	Material const& GetMaterial()const
	{
		return m_material;
	}

	bool IsTwoSided()const
	{
		return m_twoSided;
	}

	void SetTwoSided(bool value)
	{
		m_twoSided = value;
	}

	bool IsTransparent() const
	{
		return m_material.IsTransparent();
	}

private:
	ModelMaterial(const ModelMaterial&);
	ModelMaterial& operator=(const ModelMaterial&);

	TextureMap  m_textureMap1;
	Material m_material;
	bool m_twoSided;
};
