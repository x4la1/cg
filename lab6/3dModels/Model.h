#pragma once
#include "pch.h"
#include "BoundingBox.h"
#include "Mesh.h"
#include "TextureMap.h"
#include "ModelMaterial.h"
#include "Buffer.h"

class Model
{
public:
	Model()
		:m_boundingBoxMustBeUpdated(true)
	{
	}

	~Model()
	{
	}

	Mesh const& GetMesh(size_t index) const
	{
		return *m_meshes.at(index);
	}

	Mesh& GetMesh(size_t index)
	{
		return *m_meshes.at(index);
	}

	size_t GetMeshCount() const
	{
		return m_meshes.size();
	}

	Mesh& AddMesh(unsigned int vertexBufferOffset,
		unsigned int indexBufferOffset,
		unsigned vertexCount,
		unsigned indexCount,
		bool hasTexture,
		BoundingBox const& boundingBox,
		GLenum primitiveType,
		GLenum indexType)
	{
		MeshPtr pMesh(new Mesh(
			vertexBufferOffset,
			indexBufferOffset,
			vertexCount,
			indexCount,
			hasTexture,
			boundingBox,
			primitiveType,
			indexType
		));
		m_meshes.push_back(pMesh);

		m_boundingBoxMustBeUpdated = true;

		return *pMesh;
	}

	BoundingBox const GetBoundingBox() const
	{
		if (m_boundingBoxMustBeUpdated)
		{
			// Ограничивающий блок модели равен объединению ограничивающих блоков
			// всех сеток, входящих в состав модели
			BoundingBox box;
			for (size_t i = 0; i < m_meshes.size(); ++i)
			{
				box = box.Union(m_meshes[i]->GetBoundingBox());
			}
			m_boundingBox = box;

			// ограничивающий блок теперь актуален
			m_boundingBoxMustBeUpdated = false;
		}

		return m_boundingBox;
	}

	//Textures
	Texture2D& AddTextureImage(std::string const& name)
	{
		Textures::iterator it = m_textures.find(name);
		if (it != m_textures.end())
		{
			return *it->second;
		}

		Texture2DPtr pTexture(new Texture2D());

		m_textures.insert(Textures::value_type(name, pTexture));

		m_textureNames.push_back(name);
		return *pTexture;
	}

	size_t GetTexturesCount() const
	{
		return m_textures.size();
	}

	std::string GetTextureName(size_t index) const
	{
		return m_textureNames.at(index);
	}

	Texture2D& GetTextureByName(std::string const& name)
	{
		Textures::iterator it = m_textures.find(name);
		if (it == m_textures.end())
		{
			throw std::logic_error("Texture with the specified name does not exist");
		}

		return *it->second;
	}

	Texture2D const& GetTextureByName(std::string const& name) const
	{
		Textures::const_iterator it = m_textures.find(name);
		if (it == m_textures.end())
		{
			throw std::logic_error("Texture with the specified name does not exist");
		}
		return *it->second;
	}

	Texture2D& GetTexture(size_t index)
	{
		std::string textureName = m_textureNames.at(index);
		return GetTextureByName(textureName);
	}

	Texture2D const& GetTexture(size_t index) const
	{
		std::string textureName = m_textureNames.at(index);
		return GetTextureByName(textureName);
	}

	//Material
	ModelMaterial& AddMaterial()
	{
		ModelMaterialPtr pMaterial(new ModelMaterial());
		m_materials.push_back(pMaterial);
		return *pMaterial;
	}

	size_t GetMeterialCount() const
	{
		return m_materials.size();
	}

	ModelMaterial const& GetMaterial(size_t index) const
	{
		return *m_materials.at(index);
	}

	ModelMaterial& GetMaterial(size_t index)
	{
		return *m_materials.at(index);
	}

	//Buffers
	VertexBuffer& GetVertexBuffer()
	{
		return m_vertexBuffer;
	}

	VertexBuffer const& GetVertexBuffer() const
	{
		return m_vertexBuffer;
	}

	IndexBuffer& GetIndexBuffer()
	{
		return m_indexBuffer;
	}

	IndexBuffer const& GetIndexBuffer() const
	{
		return m_indexBuffer;
	}

private:
	class CompareTextureNames
	{
	public:
		bool operator()(std::string const& textureName1,
			std::string const& textureName2) const
		{
			std::string name1LowerCase(textureName1);
			std::transform(
				name1LowerCase.begin(),
				name1LowerCase.end(),
				name1LowerCase.begin(),
				tolower);

			std::string name2LowerCase(textureName2);
			std::transform(
				name2LowerCase.begin(),
				name2LowerCase.end(),
				name2LowerCase.begin(),
				tolower);

			return name1LowerCase < name2LowerCase;
		}
	};

private:
	Model(const Model&) {}
	Model& operator=(const Model&) {}

	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	typedef std::shared_ptr<ModelMaterial> ModelMaterialPtr;
	
	typedef std::shared_ptr<Texture2D> Texture2DPtr;
	typedef std::map<std::string, Texture2DPtr, CompareTextureNames> Textures;
	typedef std::vector<std::string> TextureNames;
	typedef std::shared_ptr<Mesh> MeshPtr;

	Textures m_textures;
	TextureNames m_textureNames;

	std::vector<ModelMaterialPtr> m_materials;
	std::vector<MeshPtr> m_meshes;

	mutable bool m_boundingBoxMustBeUpdated;
	mutable BoundingBox m_boundingBox;
};