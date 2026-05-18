#pragma once
#include "pch.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "TextureMap.h"
#include "Vertex.h"
#include "TextureLoader.h"
#include "Lib3dsHelpers.h"

class ModelLoader
{
public:
	class File3ds
	{
	public:
		File3ds(const char* fileName)
			:m_pFile(lib3ds_file_open(fileName))
		{
			if (m_pFile == NULL)
			{
				throw std::runtime_error(std::string("Unable to open ") + fileName);
			}
		}

		Lib3dsFile const& GetFile()const
		{
			return *m_pFile;
		}

		~File3ds()
		{
			if (m_pFile)
			{
				lib3ds_file_free(m_pFile);
			}
		}
	private:
		Lib3dsFile* m_pFile;
	};

public:
	ModelLoader()
		:m_vertexBufferUsage(GL_STATIC_DRAW)
		, m_indexBufferUsage(GL_STATIC_DRAW)
		, m_ignoreMissingTextures(true)
	{
	}

	void Load3dsFile(const char* fileName, Model& model)
	{
		Dump3dsFile(fileName);
		File3ds file(fileName);

		std::string filePath = fileName;

		size_t slashPos = filePath.find_last_of("/\\");

		std::string fileFolder =
			(slashPos == std::string::npos) ? "" :
			filePath.substr(0, slashPos + 1);

		LoadMaterials(file.GetFile(), model, fileFolder);

		LoadMeshes(file.GetFile(), model);
	}

	void IgnoreMissingTextures(bool ignoreMissingTextures)
	{
		m_ignoreMissingTextures = ignoreMissingTextures;
	}

	void SetVertexBufferUsage(GLenum vertexBufferUsage)
	{
		m_vertexBufferUsage = vertexBufferUsage;
	}

	void SetIndexBufferUsage(GLenum indexBufferUsage)
	{
		m_indexBufferUsage = indexBufferUsage;
	}

private:
	struct MeshFace
	{
		unsigned vertices[3];
		int materialIndex;
	};

	class VertexInfo
	{
	public:
		VertexInfo()
			:m_originalVertexIndex(-1)
			, m_derivedVertexIndex(-1)
			, m_normalIsDefined(false)
		{
		}

		VertexInfo(glm::dvec3 const& normal, int originalVertexIndex)
			:m_originalVertexIndex(originalVertexIndex)
			, m_derivedVertexIndex(-1)
			, m_normalIsDefined(true)
			, m_normal(normal)
		{
			assert(originalVertexIndex >= 0);
		}

		glm::dvec3 const& GetNormal()const
		{
			assert(NormalIsDefined());
			return m_normal;
		}

		void SetNormal(glm::dvec3 const& normal)
		{
			assert(!m_normalIsDefined);
			m_normal = normal;
			m_normalIsDefined = true;
		}


		bool NormalIsDefined()const
		{
			return m_normalIsDefined;
		}

		int GetOriginalVertexIndex()const
		{
			return m_originalVertexIndex;
		}

		int GetDerivedVertex()const
		{
			return m_derivedVertexIndex;
		}

		void DeriveVertex(int vertexIndex)
		{
			assert(vertexIndex >= 0);
			assert(m_derivedVertexIndex == -1);
			m_derivedVertexIndex = vertexIndex;
		}

	private:
		int m_originalVertexIndex;	
		int m_derivedVertexIndex;	
		bool m_normalIsDefined;		
		glm::dvec3 m_normal;
	};

	void LoadMaterials(
		Lib3dsFile const& file,
		Model& model,
		std::string const& baseFolder = "")
	{
		const int materialsCount = file.nmaterials;

		for (int i = 0; i < materialsCount; ++i)
		{
			Lib3dsMaterial const* pMaterial = file.materials[i];

			ModelMaterial& material = model.AddMaterial();
			Material& materialInfo = material.GetMaterial();

			{
				const float* ambient = pMaterial->ambient;
				materialInfo.SetAmbient(ambient[0], ambient[1], ambient[2]);
			}

			{
				const float* diffuse = pMaterial->diffuse;
				materialInfo.SetDiffuse(diffuse[0], diffuse[1], diffuse[2]);
			}

			{
				const float* specular = pMaterial->specular;
				materialInfo.SetSpecular(specular[0], specular[1], specular[2]);
				materialInfo.SetShininess(pMaterial->shininess);
			}

			materialInfo.SetOpacity(std::max(0.0f, std::min(1.0f, 1.0f - pMaterial->transparency)));

			{
				material.SetTwoSided(pMaterial->two_sided != 0);
			}

			LoadMaterialTextures(*pMaterial, model, material, baseFolder);
		}
	}

	void LoadMaterialTextures(
		Lib3dsMaterial const& materialInfo,
		Model& model,
		ModelMaterial& material,
		std::string const& baseFolder = "")
	{
		Lib3dsTextureMap const& tex1 = materialInfo.texture1_map;

		if (*tex1.name)
		{
			try
			{
				std::cout << "Loading texture: " << tex1.name << std::endl;

				Texture2DHandle texture1 =
					LoadTexture(tex1.name, model, baseFolder);

				TextureMap& textureMap1 = material.AddTextureMap1(texture1);

				InitTextureMap(tex1, textureMap1);
			}
			catch (std::runtime_error const&)
			{
				if (!m_ignoreMissingTextures)
				{
					throw;
				}
			}
		}

	}

	GLuint LoadTexture(
		std::string const& name,
		Model& model,
		std::string const& baseFolder = "")
	{
		Texture2D& texture = model.AddTextureImage(name);

		if (!texture)
		{
			TextureLoader loader;

			std::string textureFilePath = baseFolder + name;

			texture.Attach(
				loader.LoadTexture2D(
					std::wstring(textureFilePath.begin(),
						textureFilePath.end())
				)
			);
		}

		return texture;
	}

	void InitTextureMap(
		Lib3dsTextureMap const& textureMapInfo,
		TextureMap& textureMap)
	{
		textureMap.SetOffset(textureMapInfo.offset[0], textureMapInfo.offset[1]);
		textureMap.SetScale(textureMapInfo.scale[0], textureMapInfo.scale[1]);
		textureMap.SetRotation(textureMapInfo.rotation);
	}

	void LoadMeshes(Lib3dsFile const& file, Model& model)
	{
		std::vector<unsigned char> vertexBufferData;
		std::vector<unsigned char> indexBufferData;

		const int meshCount = file.nmeshes;

		for (int i = 0; i < meshCount; ++i)
		{
			Lib3dsMesh const& mesh = *file.meshes[i];

			LoadMesh(file.nmaterials, mesh, model, vertexBufferData, indexBufferData);
		}

		model.GetVertexBuffer().Create();
		model.GetVertexBuffer().BufferData(
			vertexBufferData.size() * sizeof(vertexBufferData[0]),
			&vertexBufferData[0],
			m_vertexBufferUsage);

		model.GetIndexBuffer().Create();
		model.GetIndexBuffer().BufferData(
			indexBufferData.size() * sizeof(indexBufferData[0]),
			&indexBufferData[0],
			m_indexBufferUsage);
	}

	static void LoadMesh(
		unsigned materialCount,
		Lib3dsMesh const& mesh,
		Model& model,
		std::vector<unsigned char>& vertexBufferData,
		std::vector<unsigned char>& indexBufferData
	)
	{

		const unsigned int vertexBufferOffset =
			static_cast<unsigned>(sizeof(unsigned char) * vertexBufferData.size());

		std::vector<MeshFace> updatedFaces;

		unsigned const numberOfVertices =
			FillVertexBufferData(mesh, vertexBufferData, updatedFaces);

		GLenum indexType = 0;

		unsigned int indexBufferOffset = 0;

		std::vector< std::vector<unsigned> > materialFaces;
		BuildMaterialFacesList(materialCount, updatedFaces, materialFaces);

		if (numberOfVertices <= UCHAR_MAX + 1)
		{
			indexType = GL_UNSIGNED_BYTE;
			indexBufferOffset = FillIndexBufferData<GLubyte>(
				updatedFaces, materialFaces, indexBufferData);
		}
		else if (numberOfVertices <= USHRT_MAX + 1)
		{
			indexType = GL_UNSIGNED_SHORT;
			indexBufferOffset = FillIndexBufferData<GLushort>(
				updatedFaces, materialFaces, indexBufferData);
		}
		else
		{
			indexType = GL_UNSIGNED_INT;
			indexBufferOffset = FillIndexBufferData<GLuint>(
				updatedFaces, materialFaces, indexBufferData);
		}

		float minMeshBound[3];
		float maxMeshBound[3];
		lib3ds_mesh_bounding_box(
			const_cast<Lib3dsMesh*>(&mesh),
			minMeshBound, maxMeshBound);

		BoundingBox meshBoundingBox(
			(glm::dvec3(minMeshBound[0], minMeshBound[1], minMeshBound[2])),
			(glm::dvec3(maxMeshBound[0], maxMeshBound[1], maxMeshBound[2])));

		Mesh& addedMesh = model.AddMesh(
			vertexBufferOffset,
			indexBufferOffset,
			numberOfVertices,
			mesh.nfaces * 3,
			mesh.texcos != NULL,
			meshBoundingBox,
			GL_TRIANGLES,
			indexType
		);

		unsigned submeshStartIndex = 0;

		for (unsigned materialIndex = 0; materialIndex <= materialCount;
			++materialIndex)
		{
			std::vector<unsigned> const& subMeshFaces =
				materialFaces[materialIndex];

			size_t const subMeshFaceCount = subMeshFaces.size();

			if (subMeshFaceCount != 0)
			{
				unsigned const subMeshIndexCount = static_cast<unsigned>(subMeshFaceCount * 3);

				unsigned subMeshIndex = addedMesh.AddSubMesh(
					submeshStartIndex, subMeshIndexCount);

				addedMesh.SetMaterialSubMesh(
					static_cast<int>(materialIndex) - 1, subMeshIndex);

				submeshStartIndex += subMeshIndexCount;
			}
		}
	}

	static unsigned FillVertexBufferData(
		Lib3dsMesh const& mesh,
		std::vector<unsigned char>& vertexBufferData,
		std::vector<MeshFace>& outputFaces
	)
	{
		const int numberOfVertices = mesh.nvertices;

		if (numberOfVertices == 0)
		{
			return 0;
		}

		float (*pInputVertices)[3] = mesh.vertices;

		float (*pInputTexCoords)[2] = mesh.texcos;

		size_t const vertexBufferOffset = vertexBufferData.size();

		if (pInputTexCoords != NULL)
		{
			vertexBufferData.resize(
				vertexBufferOffset + sizeof(TexturedVertex) * numberOfVertices);

			TexturedVertex* outputVertices =
				reinterpret_cast<TexturedVertex*>(
					&vertexBufferData[vertexBufferOffset]);

			for (int v = 0; v < numberOfVertices; ++v)
			{
				outputVertices[v].position = glm::vec3(
					pInputVertices[v][0],
					pInputVertices[v][1],
					pInputVertices[v][2]
				);

				outputVertices[v].texCoord = glm::vec2(
					pInputTexCoords[v][0],
					pInputTexCoords[v][1]
				);

				outputVertices[v].normal = glm::vec3(0.0f);
			}

			return SplitVerticesAndBuildNormals<TexturedVertex>
				(mesh, vertexBufferData, vertexBufferOffset, outputFaces);
		}
		else
		{
			vertexBufferData.resize(
				vertexBufferOffset + sizeof(Vertex) * numberOfVertices);

			Vertex* outputVertices =
				reinterpret_cast<Vertex*>(&vertexBufferData[vertexBufferOffset]);

			for (int v = 0; v < numberOfVertices; ++v)
			{
				outputVertices[v].position = glm::vec3(
					pInputVertices[v][0],
					pInputVertices[v][1],
					pInputVertices[v][2]
				);

				outputVertices[v].normal = glm::vec3(0.0f);
			}

			return SplitVerticesAndBuildNormals<Vertex>
				(mesh, vertexBufferData, vertexBufferOffset, outputFaces);
		}
	}

	template <typename IndexType>
	static unsigned FillIndexBufferData(
		std::vector<MeshFace> const& faces,
		std::vector< std::vector<unsigned> > const& materialFaces,
		std::vector<unsigned char>& indexBufferData
	)
	{
		const unsigned numberOfFaces = static_cast<unsigned>(faces.size());

		if (numberOfFaces == 0)
		{
			return 0;
		}

		unsigned indexSize = sizeof(IndexType);

		assert((indexSize & (indexSize - 1)) == 0);

		unsigned meshIndexDataSize = numberOfFaces * 3 * indexSize;

		unsigned const indexBufferOffset = static_cast<unsigned>(
			((indexBufferData.size() + indexSize - 1) / indexSize) * indexSize);

		indexBufferData.resize(indexBufferOffset + meshIndexDataSize);

		IndexType* pOutputIndex =
			reinterpret_cast<IndexType*>(&indexBufferData[indexBufferOffset]);

		IndexType const maxIndexValue = IndexType(~0);
		assert(maxIndexValue > 0);

		size_t const materialCount = materialFaces.size();
		for (
			size_t materialIndex = 0;
			materialIndex < materialCount;
			++materialIndex)
		{
			std::vector<unsigned> const& subMeshFaces =
				materialFaces[materialIndex];

			size_t const subMeshFaceCount = subMeshFaces.size();

			for (unsigned i = 0; i < subMeshFaceCount; ++i)
			{
				unsigned faceIndex = subMeshFaces[i];

				MeshFace const& inputFace = faces[faceIndex];

				for (unsigned j = 0; j < 3; ++j)
				{
					unsigned vertexIndex = inputFace.vertices[j];

					if (vertexIndex > (unsigned)maxIndexValue)
					{
						throw std::logic_error("Vertex index is out of range");
					}

					*pOutputIndex++ = static_cast<IndexType>(vertexIndex);
				}
			}
		}

		return indexBufferOffset;
	}

	static void SplitVerticesBySmoothGroup(
		Lib3dsMesh const& mesh,
		std::vector<VertexInfo>& outputVertices,
		std::vector<MeshFace>& outputFaces
	)
	{
		std::vector<float> faceVertexNormals(mesh.nfaces * 3 * 3);

		float (*pCalculatedNormals)[3] =
			reinterpret_cast<float(*)[3]>(&faceVertexNormals[0]);
		lib3ds_mesh_calculate_vertex_normals(const_cast<Lib3dsMesh*>(&mesh),
			pCalculatedNormals);

		outputVertices.clear();
		outputVertices.reserve(mesh.nfaces * 3);
		outputVertices.resize(mesh.nvertices);

		outputFaces.clear();
		outputFaces.reserve(mesh.nfaces);

		size_t calculatedNormalIndex = 0;
		for (size_t faceIndex = 0; faceIndex < mesh.nfaces; ++faceIndex)
		{
			Lib3dsFace const& face = mesh.faces[faceIndex];

			MeshFace outputFace;
			outputFace.materialIndex = face.material;

			for (unsigned i = 0; i < 3; ++i)
			{
				glm::dvec3 calculatedNormal(
					pCalculatedNormals[calculatedNormalIndex][0],
					pCalculatedNormals[calculatedNormalIndex][1],
					pCalculatedNormals[calculatedNormalIndex][2]
				);
				calculatedNormalIndex++;

				unsigned vertexIndex = face.index[i];

				if (vertexIndex >= mesh.nvertices)
				{
					throw std::runtime_error("Vertex index is out of vertex array");
				}

				while (true)
				{
					VertexInfo& vertex = outputVertices[vertexIndex];

					if (!vertex.NormalIsDefined())
					{
						vertex.SetNormal(calculatedNormal);
						break;
					}
					else
					{
						if (glm::length(calculatedNormal - vertex.GetNormal()) < 1e-5f)
						{
							break;
						}
						else
						{
							int derivedVertexIndex = vertex.GetDerivedVertex();

							if (derivedVertexIndex < 0)
							{
								VertexInfo newVertex(calculatedNormal, vertexIndex);

								vertexIndex = static_cast<int>(outputVertices.size());

								vertex.DeriveVertex(vertexIndex);

								outputVertices.push_back(newVertex);

								break;
							}
							else
							{
								vertexIndex = derivedVertexIndex;
							}
						}
					}
				}
				outputFace.vertices[i] = vertexIndex;

			}

			outputFaces.push_back(outputFace);
		}
	}

	template <class VertexType>
	static unsigned SplitVerticesAndBuildNormals(
		Lib3dsMesh const& mesh,
		std::vector<unsigned char>& vertexBufferData,
		size_t vertexBufferOffset,
		std::vector<MeshFace>& outputFaces
	)
	{
		std::vector<VertexInfo> vertices;
		SplitVerticesBySmoothGroup(mesh, vertices, outputFaces);

		size_t const numberOfVertices = vertices.size();

		vertexBufferData.resize(
			vertexBufferOffset + sizeof(VertexType) * numberOfVertices);

		VertexType* outputVertices =
			reinterpret_cast<VertexType*>(&vertexBufferData[vertexBufferOffset]);

		for (size_t vertexIndex = 0; vertexIndex < numberOfVertices; ++vertexIndex)
		{
			VertexInfo const& vertexInfo = vertices[vertexIndex];
			VertexType& outputVertex = outputVertices[vertexIndex];

			int originalVertexIndex = vertexInfo.GetOriginalVertexIndex();
			if (originalVertexIndex >= 0)
			{
				assert(static_cast<unsigned>(originalVertexIndex) < vertexIndex);
				VertexType const& originalVertex =
					outputVertices[originalVertexIndex];
				outputVertex = originalVertex;
			}

			if (vertexInfo.NormalIsDefined())
			{
				glm::dvec3 const& normal = vertexInfo.GetNormal();
				outputVertex.normal.x = normal.x;
				outputVertex.normal.y = normal.y;
				outputVertex.normal.z = normal.z;
			}
		}

		return static_cast<unsigned>(numberOfVertices);
	}

	static void BuildMaterialFacesList(
		unsigned materialCount,
		std::vector<MeshFace> const& faces,
		std::vector< std::vector<unsigned> >& materialFaces)
	{
		std::vector<unsigned> materialFaceCount(materialCount + 1);

		unsigned const faceCount = static_cast<unsigned>(faces.size());

		for (size_t faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			int faceMaterial = faces[faceIndex].materialIndex;

			if (faceMaterial < -1 ||
				faceMaterial >= static_cast<int>(materialCount))
			{
				throw std::out_of_range(
					"Face material index is out of range");
			}

			++materialFaceCount[faceMaterial + 1];
		}

		materialFaces.assign(materialCount + 1, std::vector<unsigned>());

		for (size_t materialIndex = 0;
			materialIndex <= materialCount;
			++materialIndex)
		{
			std::vector<unsigned>& faces = materialFaces[materialIndex];
			faces.reserve(materialFaceCount[materialIndex]);
		}

		for (unsigned faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			int faceMaterial = faces[faceIndex].materialIndex;

			assert(static_cast<unsigned>(faceMaterial) + 1 <= materialCount);

			std::vector<unsigned>& faces = materialFaces[faceMaterial + 1];

			faces.push_back(faceIndex);
		}
	}

private:
	GLenum m_vertexBufferUsage;
	GLenum m_indexBufferUsage;

	bool m_ignoreMissingTextures;
};
