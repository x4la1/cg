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
	// Р’С‹РїРѕР»РЅСЏРµРј Р·Р°РіСЂСѓР·РєСѓ 3ds С„Р°Р№Р»Р°, Р·Р°РїРѕР»РЅСЏСЏ РїРµСЂРµРґР°РЅРЅСѓСЋ РјРѕРґРµР»СЊ
	void Load3dsFile(const char* fileName, Model& model)
	{
		Dump3dsFile(fileName);
		// РћС‚РєСЂС‹РІР°РµРј С„Р°Р№Р»
		File3ds file(fileName);

		// РћРїРµСЂРµРґРµР»СЏРµРј РїСѓС‚СЊ Рє РєР°С‚Р°Р»РѕРіСѓ СЃ .3ds С„Р°Р№Р»РѕРј.
		// РћРЅ РїРѕРЅР°РґРѕР±РёС‚СЃСЏ РґР»СЏ РїРѕРёСЃРєР° С‚РµРєСЃС‚СѓСЂ
		std::string filePath = fileName;
		// РїРѕР·РёС†РёСЏ РєРѕСЃРѕР№ С‡РµСЂС‚С‹ (РїСЂСЏРјРѕР№, Р»РёР±Рѕ РѕР±СЂР°С‚РЅРѕР№)
		size_t slashPos = filePath.find_last_of("/\\");

		// fileFolder Р±СѓРґРµС‚ СЃРѕРґРµСЂР¶Р°С‚СЊ Р»РёР±Рѕ РїСѓСЃС‚СѓСЋ СЃС‚СЂРѕРєСѓ, Р»РёР±Рѕ РєР°С‚Р°Р»РѕРі,
		// СЃРѕРґРµСЂР¶Р°С‰РёР№ 3ds С„Р°Р№Р»
		std::string fileFolder =
			(slashPos == std::string::npos) ? "" :
			filePath.substr(0, slashPos + 1);

		// Р—Р°РіСЂСѓР¶Р°РµРј РјР°С‚РµСЂРёР°Р»С‹
		LoadMaterials(file.GetFile(), model, fileFolder);

		// Р—Р°РіСЂСѓР¶Р°РµРј РїРѕР»РёРіРѕРЅР°Р»СЊРЅС‹Рµ СЃРµС‚РєРё
		LoadMeshes(file.GetFile(), model);
	}

	// РёРіРЅРѕСЂРёСЂРѕРІР°С‚СЊ РѕС€РёР±РєРё РїСЂРё Р·Р°РіСЂСѓР·РєРµ С‚РµРєСЃС‚СѓСЂ?
	void IgnoreMissingTextures(bool ignoreMissingTextures)
	{
		m_ignoreMissingTextures = ignoreMissingTextures;
	}

	// РЈСЃС‚Р°РЅРѕРІРёС‚СЊ СЂРµР¶РёРј РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ РІРµСЂС€РёРЅРЅРѕРіРѕ Р±СѓС„РµСЂР° РјРѕРґРµР»Рё
	void SetVertexBufferUsage(GLenum vertexBufferUsage)
	{
		m_vertexBufferUsage = vertexBufferUsage;
	}

	// РЈСЃС‚Р°РЅРѕРІРёС‚СЊ СЂРµР¶РёРј РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ РёРЅРґРµРєСЃРЅРѕРіРѕ Р±СѓС„РµСЂР° РјРѕРґРµР»Рё
	void SetIndexBufferUsage(GLenum indexBufferUsage)
	{
		m_indexBufferUsage = indexBufferUsage;
	}

private:
	// РРЅС„РѕСЂРјР°С†РёСЏ Рѕ РіСЂР°РЅРё РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё, 
	// РёСЃРїРѕР»СЊР·СѓРµРјР°СЏ РїСЂРё СЂР°СЃС‰РµРїР»РµРЅРёРё РІРµСЂС€РёРЅ
	struct MeshFace
	{
		unsigned vertices[3];
		int materialIndex;
	};

	// РРЅС„РѕСЂРјР°С†РёСЏ Рѕ РІРµСЂС€РёРЅРµ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё, РёСЃРїРѕР»СЊР·СѓРµРјР°СЏ
	// РїСЂРё СЂР°СЃС‰РµРїР»РµРЅРёРё РІРµСЂС€РёРЅ
	class VertexInfo
	{
	public:
		// РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ РѕСЂРёРіРёРЅР°Р»СЊРЅРѕР№ РІРµСЂС€РёРЅС‹
		VertexInfo()
			:m_originalVertexIndex(-1)
			, m_derivedVertexIndex(-1)
			, m_normalIsDefined(false)
		{
		}

		// РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ РїРѕСЂРѕР¶РґРµРЅРЅРѕР№ РІРµСЂС€РёРЅС‹
		VertexInfo(glm::dvec3 const& normal, int originalVertexIndex)
			:m_originalVertexIndex(originalVertexIndex)
			, m_derivedVertexIndex(-1)
			, m_normalIsDefined(true)
			, m_normal(normal)
		{
			assert(originalVertexIndex >= 0);
		}

		// Р”РѕСЃС‚СѓРї Рє РІРµРєС‚РѕСЂСѓ РЅРѕСЂРјР°Р»Рё
		glm::dvec3 const& GetNormal()const
		{
			assert(NormalIsDefined());
			return m_normal;
		}

		// РџРѕР·РІРѕР»СЏРµС‚ Р·Р°РґР°С‚СЊ РЅРѕСЂРјР°Р»СЊ РґР»СЏ РІРµСЂС€РёРЅС‹
		void SetNormal(glm::dvec3 const& normal)
		{
			assert(!m_normalIsDefined);
			m_normal = normal;
			m_normalIsDefined = true;
		}


		// Р‘С‹Р»Р° Р»Рё Р·Р°РґР°РЅР° РЅРѕСЂРјР°Р»СЊ Рє РіСЂР°РЅРё?
		bool NormalIsDefined()const
		{
			return m_normalIsDefined;
		}

		// РРЅРґРµРєСЃ РѕСЂРёРіРёРЅР°Р»СЊРЅРѕР№ РІРµСЂС€РёРЅС‹, РїРѕСЂРѕРґРёРІС€РµР№ РґР°РЅРЅСѓСЋ
		// РћС‚СЂРёС†Р°С‚РµР»СЊРЅС‹Р№ РёРЅРґРµРєСЃ РѕСЂРёРіРёРЅР°Р»СЊРЅРѕР№ РІРµСЂС€РёРЅС‹ РёРјРµСЋС‚ РѕСЂРёРіРёРЅР°Р»СЊРЅС‹Рµ РІРµСЂС€РёРЅС‹
		// РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		int GetOriginalVertexIndex()const
		{
			return m_originalVertexIndex;
		}

		// Р’РѕР·РІСЂР°С‰Р°РµС‚ РёРЅРґРµРєСЃ СЃР»РµРґСѓСЋС‰РµР№ РїРѕСЂРѕР¶РґРµРЅРЅРѕР№ РІРµСЂС€РёРЅС‹ РІ СЃРїРёСЃРєРµ РІРµСЂС€РёРЅ,
		// Р»РёР±Рѕ -1, РµСЃР»Рё С‚РµРєСѓС‰Р°СЏ РІРµСЂС€РёРЅР° РЅРµ РёРјРµРµС‚ РїРѕСЂРѕР¶РґРµРЅРЅС‹С… РІРµСЂС€РёРЅ
		int GetDerivedVertex()const
		{
			return m_derivedVertexIndex;
		}

		// Р—Р°РґР°РµС‚ РёРЅРґРµРєСЃ СѓРЅР°СЃР»РµРґРѕРІР°РЅРЅРѕР№ РІРµСЂС€РёРЅС‹
		void DeriveVertex(int vertexIndex)
		{
			assert(vertexIndex >= 0);
			assert(m_derivedVertexIndex == -1);
			m_derivedVertexIndex = vertexIndex;
		}

	private:
		int m_originalVertexIndex;	// РёРЅРґРµРєСЃ Р±Р°Р·РѕРІРѕР№ РІРµСЂС€РёРЅС‹
		int m_derivedVertexIndex;	// РёРЅРґРµРєСЃ РїРѕСЂРѕР¶РґРµРЅРЅРѕР№ РІРµСЂС€РёРЅС‹
		bool m_normalIsDefined;		// Р·Р°РґР°РЅ Р»Рё РІРµРєС‚РѕСЂ РЅРѕСЂРјР°Р»Рё?
		glm::dvec3 m_normal;
	};

	// Р—Р°РіСЂСѓР¶Р°РµРј РјР°С‚РµСЂРёР°Р»С‹
	void LoadMaterials(
		Lib3dsFile const& file,
		Model& model,
		std::string const& baseFolder = "")
	{
		const int materialsCount = file.nmaterials;

		for (int i = 0; i < materialsCount; ++i)
		{
			Lib3dsMaterial const* pMaterial = file.materials[i];

			// Р”РѕР±Р°РІР»СЏРµРј РЅРѕРІС‹Р№ РјР°С‚РµСЂРёР°Р» Рє РјРѕРґРµР»Рё
			ModelMaterial& material = model.AddMaterial();
			// Рё РїРѕР»СѓС‡Р°РµРј СЃРІСЏР·Р°РЅРЅРѕРµ СЃ СЌС‚РёРј РјР°С‚РµСЂРёР»РѕРј РѕРїРёСЃР°РЅРёРµ
			Material& materialInfo = material.GetMaterial();

			// Р—Р°РґР°РµРј С„РѕРЅРѕРІС‹Р№ С†РІРµС‚ РјР°С‚РµСЂРёР°Р»Р°
			{
				const float* ambient = pMaterial->ambient;
				materialInfo.SetAmbient(ambient[0], ambient[1], ambient[2]);
			}

			// Р—Р°РґР°РµРј РґРёС„С„СѓР·РЅС‹Р№ С†РІРµС‚ РјР°С‚РµСЂРёР°Р»Р°
			{
				const float* diffuse = pMaterial->diffuse;
				materialInfo.SetDiffuse(diffuse[0], diffuse[1], diffuse[2]);
			}

			// Р—Р°РґР°РµРј Р·РµСЂРєР°Р»СЊРЅС‹Р№ С†РІРµС‚ РјР°С‚РµСЂРёР°Р»Р° Рё СЃС‚РµРїРµРЅСЊ Р±Р»РµСЃРєР°
			{
				const float* specular = pMaterial->specular;
				materialInfo.SetSpecular(specular[0], specular[1], specular[2]);
				materialInfo.SetShininess(pMaterial->shininess);
			}

			// In .3ds transparency is stored as "how transparent",
			// but OpenGL blending expects the opposite value in alpha.
			materialInfo.SetOpacity(std::max(0.0f, std::min(1.0f, 1.0f - pMaterial->transparency)));

			{
				material.SetTwoSided(pMaterial->two_sided != 0);
			}

			// Р—Р°РіСЂСѓР¶Р°РµРј С‚РµРєСЃС‚СѓСЂС‹ РјР°С‚РµСЂРёР°Р»Р°
			LoadMaterialTextures(*pMaterial, model, material, baseFolder);
		}
	}

	// Р—Р°РіСЂСѓР¶Р°РµРј С‚РµРєСЃС‚СѓСЂС‹, СЃРІСЏР·Р°РЅРЅС‹Рµ СЃ РјР°С‚РµСЂРёР°Р»РѕРј
	void LoadMaterialTextures(
		Lib3dsMaterial const& materialInfo,
		Model& model,
		ModelMaterial& material,
		std::string const& baseFolder = "")
	{
		Lib3dsTextureMap const& tex1 = materialInfo.texture1_map;

		// .3ds stores texture names as C strings in a fixed-size buffer,
		// so check the first character instead of comparing pointers.
		if (*tex1.name)
		{
			try
			{
				std::cout << "Loading texture: " << tex1.name << std::endl;

				// Р—Р°РіСЂСѓР¶Р°РµРј С‚РµРєСЃС‚СѓСЂСѓ
				Texture2DHandle texture1 =
					LoadTexture(tex1.name, model, baseFolder);

				// Р”РѕР±Р°РІР»СЏРµРј С‚РµРєСЃС‚СѓСЂРЅСѓСЋ РєР°СЂС‚Сѓ
				TextureMap& textureMap1 = material.AddTextureMap1(texture1);
				// РќР°РїРѕР»РЅСЏРµРј РµРµ СЃРІРµРґРµРЅРёСЏРјРё РёР· .3ds С„Р°Р№Р»Р°
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

	// Р—Р°РіСЂСѓР¶Р°РµРј С‚РµРєСЃС‚СѓСЂСѓ Рё РґРѕР±Р°РІР»СЏРµРј РµРµ РІ РјРѕРґРµР»СЊ
	GLuint LoadTexture(
		std::string const& name,
		Model& model,
		std::string const& baseFolder = "")
	{
		Texture2D& texture = model.AddTextureImage(name);
		// РќР°Рј РІРµСЂРЅСѓС‚ Р»РёР±Рѕ СЃСЃС‹Р»РєСѓ РЅР° СЃСѓС‰РµСЃС‚РІСѓСЋС‰РёР№ С‚РµРєСЃС‚СѓСЂРЅС‹Р№ РѕР±СЉРµРєС‚,
		// Р»РёР±Рѕ СЃСЃС‹Р»РєСѓ РЅР° РІРЅРѕРІСЊ СЃРѕР·РґР°РЅРЅС‹Р№

		if (!texture)	// Р•СЃР»Рё РґР»СЏ С‚РµРєСЃС‚СѓСЂС‹ РµС‰Рµ РЅРµ Р·Р°РґР°РЅРѕ С‚РµРєСЃС‚СѓСЂРЅРѕРµ РёР·РѕР±СЂР°Р¶РµРЅРёРµ
		{
			TextureLoader loader;

			std::string textureFilePath = baseFolder + name;

			// Р—Р°РіСЂСѓР¶Р°РµРј С‚РµРєСЃС‚СѓСЂРЅРѕРµ РёР·РѕР±СЂР°Р¶РµРЅРёРµ Рё РїСЂРёСЃРѕРµРґРёРЅСЏРµРј РµРіРѕ Рє С‚РµРєСЃС‚СѓСЂРµ
			// РР·-Р·Р° РїСЂРѕСЃС‚РµР№С€РµРіРѕ РїРµСЂРµРІРѕРґР° РёРјРµРЅРё РёР· string РІ wstring 
			// РєРѕСЂСЂРµРєС‚РЅРѕ Р·Р°РіСЂСѓР¶Р°С‚СЊСЃСЏ Р±СѓРґСѓС‚ С‚РѕР»СЊРєРѕ С„Р°Р№Р»С‹, РІ РїСѓС‚Рё РєРѕС‚РѕСЂС‹С… РЅРµ СЃРѕРґРµСЂР¶Р°С‚СЃСЏ 
			// СЃРёРјРІРѕР»С‹ Р·Р° РїСЂРµРґРµР»Р°РјРё РєРѕРґРѕРІРѕР№ С‚Р°Р±Р»РёС†С‹ ASCII
			texture.Attach(
				loader.LoadTexture2D(
					std::wstring(textureFilePath.begin(),
						textureFilePath.end())
				)
			);
		}

		return texture;
	}

	// РРЅРёС†РёР°Р»РёР·РёСЂСѓРµРј С‚РµРєСЃС‚СѓСЂРЅСѓСЋ РєР°СЂС‚Сѓ РґР°РЅРЅС‹РјРё РёР· .3ds С„Р°Р№Р»Р°
	void InitTextureMap(
		Lib3dsTextureMap const& textureMapInfo,
		TextureMap& textureMap)
	{
		textureMap.SetOffset(textureMapInfo.offset[0], textureMapInfo.offset[1]);
		textureMap.SetScale(textureMapInfo.scale[0], textureMapInfo.scale[1]);
		textureMap.SetRotation(textureMapInfo.rotation);
	}

	// Р—Р°РіСЂСѓР¶Р°РµРј РїРѕР»РёРіРѕРЅР°Р»СЊРЅС‹Рµ СЃРµС‚РєРё
	void LoadMeshes(Lib3dsFile const& file, Model& model)
	{
		std::vector<unsigned char> vertexBufferData;
		std::vector<unsigned char> indexBufferData;

		const int meshCount = file.nmeshes;

		for (int i = 0; i < meshCount; ++i)
		{
			Lib3dsMesh const& mesh = *file.meshes[i];

			// Р”РѕР±Р°РІР»СЏРµРј РґР°РЅРЅС‹Рµ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё РёР· 3ds С„Р°Р№Р»Р°
			// Рє С‚РµРєСѓС‰РµР№ РјРѕРґРµР»Рё, Р° РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ РІРµСЂС€РёРЅР°С… Рё РёРЅРґРµРєСЃР°С…
			// РґРѕР±Р°РІР»СЏРµРј РІ РјР°СЃСЃРёРІС‹ РІРµСЂС€РёРЅ Рё РёРЅРґРµРєСЃРѕРІ
			LoadMesh(file.nmaterials, mesh, model, vertexBufferData, indexBufferData);
		}

		// РЎРѕР·РґР°РµРј РІРµСЂС€РёРЅРЅС‹Р№ Р±СѓС„РµСЂ
		model.GetVertexBuffer().Create();
		// Рё Р·Р°РїРѕР»РЅСЏРµРј РµРіРѕ РґР°РЅРЅС‹РјРё Рѕ РІРµСЂС€РёРЅР°С…, СЃРѕР±СЂР°РЅРЅС‹РјРё СЃРѕ РІСЃРµС… 
		// РїРѕР»РёРіРѕРЅР°Р»СЊРЅС‹С… СЃРµС‚РѕРє РјРѕРґРµР»Рё
		model.GetVertexBuffer().BufferData(
			vertexBufferData.size() * sizeof(vertexBufferData[0]),
			&vertexBufferData[0],
			m_vertexBufferUsage);

		// РЎРѕР·РґР°РµРј Р±СѓС„РµСЂ РёРЅРґРµРєСЃРѕРІ
		model.GetIndexBuffer().Create();
		// Рё Р·Р°РїРѕР»РЅСЏРµРј РµРіРѕ РґР°РЅРЅС‹РјРё РѕР± РёРЅРґРµРєСЃР°С… РІРµСЂС€РёРЅ, СЃРѕСЃС‚Р°РІР»СЏСЋС‰РёС… РіСЂР°РЅРё,
		// СЃРѕР±СЂР°РЅРЅС‹РјРё СЃРѕ РІСЃРµС… РїРѕР»РёРіРѕРЅР°Р»СЊРЅС‹С… СЃРµС‚РѕРє РјРѕРґРµР»Рё
		model.GetIndexBuffer().BufferData(
			indexBufferData.size() * sizeof(indexBufferData[0]),
			&indexBufferData[0],
			m_indexBufferUsage);
	}

	// Р”РѕР±Р°РІР»СЏРµРј Рє РјРѕРґРµР»Рё РїРѕР»РёРіРѕРЅР°Р»СЊРЅСѓСЋ СЃРµС‚РєСѓ Рё Р·Р°РїРѕР»РЅСЏРµРј
	// РїРµСЂРµРґР°РЅРЅС‹Рµ РјР°СЃСЃРёРІС‹ РІРµСЂС€РёРЅ Рё РёРЅРґРµРєСЃРѕРІ РґР°РЅРЅС‹РјРё РёР· 3ds С„Р°Р№Р»Р°
	static void LoadMesh(
		unsigned materialCount,
		Lib3dsMesh const& mesh,
		Model& model,
		std::vector<unsigned char>& vertexBufferData,
		std::vector<unsigned char>& indexBufferData
	)
	{
		// Р’С‹С‡РёСЃР»СЏРµРј СЃРјРµС‰РµРЅРёРµ РІ Р±СѓС„РµСЂРµ РІРµСЂС€РёРЅ С‚РµРєСѓС‰РµР№ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		const unsigned int vertexBufferOffset =
			static_cast<unsigned>(sizeof(unsigned char) * vertexBufferData.size());

		// РћР±РЅРѕРІР»РµРЅРЅС‹Р№ РјР°СЃСЃРёРІ РіСЂР°РЅРµР№
		std::vector<MeshFace> updatedFaces;

		// Р—Р°РїРѕР»РЅСЏРµРј РјР°СЃСЃРёРІ РІРµСЂС€РёРЅ, РІС‹РїРѕР»РЅСЏСЏ РїСЂРё РЅРµРѕР±С…РѕРґРёРјРѕСЃС‚Рё 
		// РёС… СЂР°СЃС‰РµРїР»РµРЅРёРµ
		unsigned const numberOfVertices =
			FillVertexBufferData(mesh, vertexBufferData, updatedFaces);

		// РўРёРї, РёСЃРїРѕР»СЊР·СѓРµРјС‹Р№ РґР»СЏ С…СЂР°РЅРµРЅРёСЏ РёРЅРґРµРєСЃРѕРІ РІРµСЂС€РёРЅ
		GLenum indexType = 0;

		// РРЅРёС†РёР°Р»РёР·РёСЂСѓРµРј СЃРјРµС‰РµРЅРёРµ Рє РґР°РЅРЅС‹Рј РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		// РІ Р±СѓС„РµСЂРµ РёРЅРґРµРєСЃРѕРІ
		unsigned int indexBufferOffset = 0;

		// РЎС‚СЂРѕРёРј СЃРїРёСЃРєРё РіСЂР°РЅРµР№, СЃРіСЂСѓРїРїРёСЂРѕРІР°РЅРЅС‹Рµ РїРѕ РёСЃРїРѕР»СЊР·СѓРµРјРѕРјСѓ
		// РёРЅРґРµРєСЃСѓ РјР°С‚РµСЂРёР°Р»Р°
		std::vector< std::vector<unsigned> > materialFaces;
		BuildMaterialFacesList(materialCount, updatedFaces, materialFaces);

		// Р’ Р·Р°РІРёСЃРёРјРѕСЃС‚Рё РѕС‚ РєРѕР»РёС‡РµСЃС‚РІР° РІРµСЂС€РёРЅ РїРѕСЃР»Рµ СЂР°СЃС‰РµРїР»РµРЅРёСЏ
		// СЃРѕС…СЂР°РЅСЏРµРј РёРЅРґРµРєСЃС‹ РІ РІРёРґРµ 8, 16 РёР»Рё 32 Р±РёС‚РЅС‹С… С‡РёСЃРµР»
		if (numberOfVertices <= UCHAR_MAX + 1)	// РґРѕСЃС‚Р°С‚РѕС‡РЅРѕ 8 Р±РёС‚?
		{
			indexType = GL_UNSIGNED_BYTE;
			indexBufferOffset = FillIndexBufferData<GLubyte>(
				updatedFaces, materialFaces, indexBufferData);
		}
		else if (numberOfVertices <= USHRT_MAX + 1) // РґРѕСЃС‚Р°С‚РѕС‡РЅРѕ 16 Р±РёС‚?
		{
			indexType = GL_UNSIGNED_SHORT;
			indexBufferOffset = FillIndexBufferData<GLushort>(
				updatedFaces, materialFaces, indexBufferData);
		}
		else	// РСЃРїРѕР»СЊР·СѓРµРј 32 Р±РёС‚РЅС‹Рµ РёРЅРґРµРєСЃС‹
		{
			indexType = GL_UNSIGNED_INT;
			indexBufferOffset = FillIndexBufferData<GLuint>(
				updatedFaces, materialFaces, indexBufferData);
		}

		// Р’С‹С‡РёСЃР»СЏРµРј РѕРіСЂР°РЅРёС‡РёРІР°СЋС‰РёР№ Р±Р»РѕРє С‚РµРєСѓС‰РµР№ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		// РїСЂРё РїРѕРјРѕС‰Рё СЃСЂРµРґСЃС‚РІ Р±РёР±Р»РёРѕС‚РµРєРё lib3ds
		float minMeshBound[3];
		float maxMeshBound[3];
		lib3ds_mesh_bounding_box(
			const_cast<Lib3dsMesh*>(&mesh),
			minMeshBound, maxMeshBound);

		// РЎРѕР·РґР°РµРј Bounding box РЅР° РѕСЃРЅРѕРІРµ РґР°РЅРЅС‹С…, РІРѕР·РІСЂР°С‰РµРЅРЅС‹С… lib3ds
		BoundingBox meshBoundingBox(
			(glm::dvec3(minMeshBound[0], minMeshBound[1], minMeshBound[2])),
			(glm::dvec3(maxMeshBound[0], maxMeshBound[1], maxMeshBound[2])));

		// Р”РѕР±Р°РІР»СЏРµРј Рє РјРѕРґРµР»Рё РїРѕР»РёРіРѕРЅР°Р»СЊРЅСѓСЋ СЃРµС‚РєСѓ
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

		// РџРѕСЂСЏРґРєРѕРІС‹Р№ РЅРѕРјРµСЂ РёРЅРґРµРєСЃР° СЃР°РјРѕР№ РїРµСЂРІРѕР№ РїРѕРґСЃРµС‚РєРё СЂР°РІРµРЅ 0
		unsigned submeshStartIndex = 0;

		// Р”РѕР±Р°РІР»СЏРµРј РІ СЃРµС‚СѓРє РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ СЃРѕРґРµСЂР¶Р°С‰РёС…СЃСЏ РІ РЅРµР№ РїРѕРґСЃРµС‚РєР°С…
		// Рё РјР°С‚РµСЂРёР°Р»Р°С…
		for (unsigned materialIndex = 0; materialIndex <= materialCount;
			++materialIndex)
		{
			// РњР°СЃСЃРёРІ РіСЂР°РЅРµР№, РёСЃРїРѕР»СЊР·СѓСЋС‰РёС… РјР°С‚РµСЂРёР°Р» materialIndex
			std::vector<unsigned> const& subMeshFaces =
				materialFaces[materialIndex];

			// РєРѕР»РёС‡РµСЃС‚РІРѕ РіСЂР°РЅРµР№, РёРјРµСЋС‰РёС… РјР°С‚РµСЂРёР°Р» materialIndex
			size_t const subMeshFaceCount = subMeshFaces.size();

			// Р•СЃР»Рё РїРѕРґСЃРµС‚РєР° РЅРµ РїСѓСЃС‚Р°СЏ, С‚Рѕ РґРѕР±Р°РІР»СЏРµРј РµРµ РІ РІРёРґРµ РїРѕРґСЃРµС‚РєРё
			if (subMeshFaceCount != 0)
			{
				// Рў.Рє. РіСЂР°РЅРё С‚СЂРµСѓРіРѕР»СЊРЅС‹Р№, РєРѕР»РёС‡РµСЃС‚РІРѕ РёРЅРґРµРєСЃРѕРІ РІ 3 СЂР°Р·Р° Р±РѕР»СЊС€Рµ
				// РєРѕР»РёС‡РµСЃС‚РІР° РіСЂР°РЅРµР№
				unsigned const subMeshIndexCount = static_cast<unsigned>(subMeshFaceCount * 3);

				// Р”РѕР±Р°РІР»СЏРµРј РїРѕРґСЃРµС‚РєСѓ
				unsigned subMeshIndex = addedMesh.AddSubMesh(
					submeshStartIndex, subMeshIndexCount);

				// Р—Р°РґР°РµРј РІ СЃРµС‚РєРµ СЃРІСЏР·СЊ РјРµР¶РґСѓ С‚РµРєСѓС‰РёРј РёРЅРґРµРєСЃРѕРј РјР°С‚РµСЂРёР°Р»Р° Рё 
				// РґРѕР±Р°РІР»РµРЅРЅРѕР№ РїРѕРґСЃРµС‚РєРѕР№
				addedMesh.SetMaterialSubMesh(
					static_cast<int>(materialIndex) - 1, subMeshIndex);

				// РІС‹С‡РёСЃР»СЏРµРј РїРѕСЂСЏРґРєРѕРІС‹Р№ РЅРѕРјРµСЂ РёРЅРґРµРєСЃР° СЃР»РµРґСѓСЋС‰РµР№ РїРѕРґСЃРµС‚РєРё
				submeshStartIndex += subMeshIndexCount;
			}
		}
	}

	// Р—Р°РїРѕР»РЅСЏРµРј РјР°СЃСЃРёРІ РІРµСЂС€РёРЅ РґР°РЅРЅС‹РјРё РёР· .3ds С„Р°Р№Р»Р°
	// РІС‹С‡РёСЃР»СЏСЏ РЅРѕСЂРјР°Р»Рё Рє РІРµСЂС€РёРЅР°Рј, РїРѕСЂРѕР¶РґР°СЏ РЅРѕРІС‹Рµ РІРµСЂС€РёРЅС‹
	// Рё РѕР±РЅРѕРІР»СЏСЏ РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ РіСЂР°РЅСЏС… РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
	// Р’РѕР·РІСЂР°С‰Р°РµРј РєРѕР»РёС‡РµСЃС‚РІРѕ РІРµСЂС€РёРЅ РІ СЂРµР·СѓР»СЊС‚РёСЂСѓСЋС‰РµР№ СЃРµС‚РєРµ
	static unsigned FillVertexBufferData(
		Lib3dsMesh const& mesh,
		std::vector<unsigned char>& vertexBufferData,
		std::vector<MeshFace>& outputFaces
	)
	{
		const int numberOfVertices = mesh.nvertices;

		if (numberOfVertices == 0)
		{
			return 0; // Р’РµСЂС€РёРЅ РЅРµС‚, РЅРёС‡РµРіРѕ РЅРµ РґРѕР±Р°РІР»СЏРµРј, РІРѕР·РІСЂР°С‰Р°РµРј 0
		}

		// Р°РґСЂРµСЃ РјР°СЃСЃРёРІР° РІРµСЂС€РёРЅ РІ 3ds-С„Р°Р№Р»Рµ
		float (*pInputVertices)[3] = mesh.vertices;

		// Р°РґСЂРµСЃ РјР°СЃСЃРёРІР° С‚РµРєСЃС‚СѓСЂРЅС‹С… РєРѕРѕСЂРґРёРЅР°С‚ РІ 3ds С„Р°Р№Р»Рµ
		float (*pInputTexCoords)[2] = mesh.texcos;

		// СЃРјРµС‰РµРЅРёРµ РґРѕ РЅР°С‡Р°Р»Р° РґР°РЅРЅС‹С… РІ 3ds С„Р°Р№Р»Рµ
		size_t const vertexBufferOffset = vertexBufferData.size();

		// РїСЂРѕРІРµСЂСЏРµРј, РµСЃС‚СЊ Р»Рё Сѓ СЃРµС‚РєРё С‚РµРєСЃС‚СѓСЂРЅС‹Рµ РєРѕРѕСЂРґРёРЅР°С‚С‹
		if (pInputTexCoords != NULL)
		{
			// СЃРµС‚РєР° СЃ С‚РµРєСЃС‚СѓСЂРЅС‹РјРё РєРѕРѕСЂРґРёРЅР°С‚Р°РјРё

			// СѓРІРµР»РёС‡РёРІР°РµРј РјР°СЃСЃРёРІ РЅР° СЂР°Р·РјРµСЂ, Р·Р°РЅРёРјР°РµРјС‹С… РІРµСЂС€РёРЅР°РјРё 
			// СЃ С‚РµРєСЃС‚СѓСЂРЅС‹РјРё РєРѕРѕСЂРґРёРЅР°С‚Р°РјРё
			vertexBufferData.resize(
				vertexBufferOffset + sizeof(TexturedVertex) * numberOfVertices);

			// Р·Р°РґР°РµРј Р°РґСЂРµСЃ СЂР°СЃРїРѕР»РѕР¶РµРЅРёСЏ РІРµСЂС€РёРЅРЅС‹С… РґР°РЅРЅС‹С… РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
			TexturedVertex* outputVertices =
				reinterpret_cast<TexturedVertex*>(
					&vertexBufferData[vertexBufferOffset]);

			// Р—Р°РїРѕР»РЅСЏРµРј РјР°СЃСЃРёРІ РґР°РЅРЅС‹РјРё Рѕ РІРµСЂС€РёРЅР°С… РІ С„РѕСЂРјР°С‚Рµ СЃС‚СЂСѓРєС‚СѓСЂС‹ TexturedVertex
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

				// РќРѕСЂРјР°Р»Рё РїРѕРєР° СЃР±СЂР°СЃС‹РІР°РµРј (РѕРЅРё Р±СѓРґСѓС‚ РїРµСЂРµР·Р°РїРёСЃР°РЅС‹ РІ SplitVerticesAndBuildNormals)
				outputVertices[v].normal = glm::vec3(0.0f);
			}
			// Р”РѕР±Р°РІР»СЏРµРј РЅРµРґРѕСЃС‚Р°СЋС‰СѓСЋ РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ РЅРѕСЂРјР°Р»СЏС… Рё РїРѕСЂРѕР¶РґРµРЅРЅС‹С… РІРµСЂС€РёРЅР°С…
			// Рё РІРѕР·РІСЂР°С‰Р°РµРј РєРѕР»РёС‡РµСЃС‚РІРѕ РїРѕР»СѓС‡РµРЅРЅС‹С… РІРµСЂС€РёРЅ РІ СЃРµС‚РєРµ
			return SplitVerticesAndBuildNormals<TexturedVertex>
				(mesh, vertexBufferData, vertexBufferOffset, outputFaces);
		}
		else	// СЃРµС‚РєР° Р±РµР· С‚РµРєСЃС‚СѓСЂРЅС‹С… РєРѕРѕСЂРґРёРЅР°С‚
		{
			// СѓРІРµР»РёС‡РёРІР°РµРј РјР°СЃСЃРёРІ РЅР° СЂР°Р·РјРµСЂ, Р·Р°РЅРёРјР°РµРјС‹С… РІРµСЂС€РёРЅР°РјРё 
			// Р±РµР· С‚РµРєСЃС‚СѓСЂРЅС‹С… РєРѕРѕСЂРґРёРЅР°С‚
			vertexBufferData.resize(
				vertexBufferOffset + sizeof(Vertex) * numberOfVertices);
			std::cout << "pizdec\n";
			// Р·Р°РґР°РµРј Р°РґСЂРµСЃ СЂР°СЃРїРѕР»РѕР¶РµРЅРёСЏ РІРµСЂС€РёРЅРЅС‹С… РґР°РЅРЅС‹С… РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
			Vertex* outputVertices =
				reinterpret_cast<Vertex*>(&vertexBufferData[vertexBufferOffset]);
			std::cout << "pizdec1\n";
			// Р—Р°РїРѕР»РЅСЏРµРј РјР°СЃСЃРёРІ РґР°РЅРЅС‹РјРё Рѕ РІРµСЂС€РёРЅР°С… РІ С„РѕСЂРјР°С‚Рµ СЃС‚СЂСѓРєС‚СѓСЂС‹ Vertex
			for (int v = 0; v < numberOfVertices; ++v)
			{
				outputVertices[v].position = glm::vec3(
					pInputVertices[v][0],
					pInputVertices[v][1],
					pInputVertices[v][2]
				);

				outputVertices[v].normal = glm::vec3(0.0f);
			}
			// Р”РѕР±Р°РІР»СЏРµРј РЅРµРґРѕСЃС‚Р°СЋС‰СѓСЋ РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ РЅРѕСЂРјР°Р»СЏС… Рё РїРѕСЂРѕР¶РґРµРЅРЅС‹С… РІРµСЂС€РёРЅР°С…
			// Рё РІРѕР·РІСЂР°С‰Р°РµРј РєРѕР»РёС‡РµСЃС‚РІРѕ РїРѕР»СѓС‡РµРЅРЅС‹С… РІРµСЂС€РёРЅ РІ СЃРµС‚РєРµ
			return SplitVerticesAndBuildNormals<Vertex>
				(mesh, vertexBufferData, vertexBufferOffset, outputFaces);
		}
	}

	// Р—Р°РїРѕР»РЅСЏРµРј РјР°СЃСЃРёРІ РёРЅРґРµРєСЃРѕРІ РІРµСЂС€РёРЅ
	// Р’ РєР°С‡РµСЃС‚РІРµ С€Р°Р±Р»РѕРЅРЅРѕРіРѕ РїР°СЂР°РјРµС‚СЂР° IndexType РїСЂРёРЅРёРјР°РµС‚СЃСЏ
	// С†РµР»РѕС‡РёСЃР»РµРЅРЅС‹Р№ С‚РёРї, РёСЃРїРѕР»СЊР·СѓРµРјС‹С… РґР»СЏ С…СЂР°РЅРµРЅРёСЏ РёРЅРґРµРєСЃРѕРІ.
	// Р’РѕР·РІСЂР°С‰Р°РµРј СЃРјРµС‰РµРЅРёРµ Рє РёРЅРґРµРєСЃР°Рј СЃРµС‚РєРё РІРЅСѓС‚СЂРё
	// РјР°СЃСЃРёРІР° indexBufferData
	template <typename IndexType>
	static unsigned FillIndexBufferData(
		std::vector<MeshFace> const& faces,
		std::vector< std::vector<unsigned> > const& materialFaces,
		std::vector<unsigned char>& indexBufferData
	)
	{
		// РєРѕР»РёС‡РµСЃС‚РІРѕ РіСЂР°РЅРµР№
		const unsigned numberOfFaces = static_cast<unsigned>(faces.size());

		if (numberOfFaces == 0)
		{
			return 0; // Р’РµСЂС€РёРЅ РЅРµС‚, РЅРёС‡РµРіРѕ РЅРµ РґРѕР±Р°РІР»СЏРµРј, РІРѕР·РІСЂР°С‰Р°РµРј 0
		}

		// СЂР°Р·РјРµСЂ (РІ Р±Р°Р№С‚Р°С…), С‚СЂРµР±СѓРµРјС‹С… РґР»СЏ С…СЂР°РЅРµРЅРёСЏ РѕРґРЅРѕРіРѕ РёРЅРґРµРєСЃР°
		unsigned indexSize = sizeof(IndexType);

		// РџСЂРѕРІРµСЂРєР° РЅР° С‚Рѕ, С‡С‚Рѕ indexSize - СЃС‚РµРїРµРЅСЊ РґРІРѕР№РєРё
		assert((indexSize & (indexSize - 1)) == 0);

		// Р Р°Р·РјРµСЂ РґР°РЅРЅС‹С…, Р·Р°РЅРёРјР°РµРјС‹Р№ РІСЃРµРјРё РёРЅРґРµРєСЃР°РјРё РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		unsigned meshIndexDataSize = numberOfFaces * 3 * indexSize;

		// Р’С‹С‡РёСЃР»СЏРµРј СЃРјРµС‰РµРЅРёРµ РІ Р±СѓС„РµСЂРµ РёРЅРґРµРєСЃРѕРІ С‚Р°РєРёРј РѕР±СЂР°Р·РѕРј, С‡С‚РѕР±С‹ РёРЅРґРµРєСЃС‹
		// СЃРµС‚РєРё РЅР°С…РѕРґРёР»РёСЃСЊ РїРѕ СЃРјРµС‰РµРЅРёСЋ, РєСЂР°С‚РЅРѕРјСѓ СЂР°Р·РјРµСЂСѓ РёРЅРґРµРєСЃР°
		// (РґР»СЏ СѓСЃРєРѕСЂРµРЅРёСЏ РґРѕСЃС‚СѓРїР° Рє РґР°РЅРЅС‹Рј)
		// 8-Р±РёС‚РЅС‹Рµ РёРЅРґРµРєСЃС‹ Р±СѓРґСѓС‚ РІС‹СЂРѕРІРЅРµРЅС‹ РїРѕ РіСЂР°РЅРёС†Рµ Р±Р°Р№С‚РѕРІ
		// 16-Р±РёС‚РЅС‹Рµ РёРЅРґРµРєСЃС‹ - РїРѕ РґРІСѓС…Р±Р°Р№С‚РЅРѕР№ РіСЂР°РЅРёС†Рµ
		// 32-Р±РёС‚РЅС‹Рµ РёРЅРґРµРєСЃС‹ - РїРѕ СЃРјРµС‰РµРЅРёСЋ, РєСЂР°С‚РЅРѕРјСѓ 4
		unsigned const indexBufferOffset = static_cast<unsigned>(
			((indexBufferData.size() + indexSize - 1) / indexSize) * indexSize);

		// РЈРІРµР»РёС‡РёРІР°РµРј СЂР°Р·РјРµСЂ Р±СѓС„РµСЂР° РёРЅРґРµРєСЃРѕРІ С‚Р°Рє, С‡С‚РѕР±С‹ РїРѕ СЃРјРµС‰РµРЅРёСЋ
		// indexBufferOffset СЂР°Р·РјРµСЃС‚РёС‚СЊ РґР°РЅРЅС‹Рµ СЂР°Р·РјРµСЂРѕРј meshIndexDataSize
		indexBufferData.resize(indexBufferOffset + meshIndexDataSize);

		// РЈРєР°Р·Р°С‚РµР»СЊ РЅР° РїРѕР·РёС†РёСЋ РІ РјР°СЃСЃРёРІРµ РёРЅРґРµРєСЃРѕРІ, СЃРѕРѕС‚РІРµС‚СЃС‚РІСѓСЋС‰РµР№ РЅР°С‡Р°Р»Сѓ
		// РґР°РЅРЅС‹С… С‚РµРєСѓС‰РµР№ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		IndexType* pOutputIndex =
			reinterpret_cast<IndexType*>(&indexBufferData[indexBufferOffset]);

		// РР·-Р·Р° РєРѕРЅС„Р»РёРєС‚РѕРІ РјРµР¶РґСѓ РјР°РєСЂРѕСЃРѕРј max РёР· windows.h 
		// Рё РјРµС‚РѕРґРѕРј std::numeric_limits::max() РїСЂРёРґРµС‚СЃСЏ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ
		// СЃР»РµРґСѓСЋС‰РёР№ СЃРїРѕСЃРѕР± РѕРїРµСЂРµРґРµР»РµРЅРёСЏ РјР°РєСЃРёРјР°Р»СЊРЅРѕРіРѕ Р·РЅР°С‡РµРЅРёСЏ Р±РµР·Р·РЅР°РєРѕРІРѕРіРѕ
		// С†РµР»РѕС‡РёСЃР»РµРЅРЅРѕРіРѕ С‚РёРїР° IndexType
		IndexType const maxIndexValue = IndexType(~0);
		// Р”РѕРїРѕР»РЅРёС‚РµР»СЊРЅР°СЏ РїСЂРѕРІРµСЂРєР° РЅР° С‚Рѕ, С‡С‚Рѕ РґР»СЏ С…СЂР°РЅРµРЅРёСЏ РёРЅРґРµРєСЃРѕРІ
		// РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ Р±РµР·Р·РЅР°РєРѕРІС‹Р№ С‚РёРї
		assert(maxIndexValue > 0);

		size_t const materialCount = materialFaces.size();
		// РџСЂРѕР±РµРіР°РµРјСЃСЏ РїРѕ СЃРїРёСЃРєСѓ РјР°С‚РµСЂРёР°Р»РѕРІ
		for (
			size_t materialIndex = 0;
			materialIndex < materialCount;
			++materialIndex)
		{
			// РњР°СЃСЃРёРІ РіСЂР°РЅРµР№, РІС…РѕРґСЏС‰РёС… РІ СЃРѕСЃС‚Р°РІ РїРѕРґСЃРµС‚РєРё СЃ С‚РµРєСѓС‰РёРј РёРЅРґРµРєСЃРѕРј
			// РјР°С‚РµСЂРёР°Р»Р°
			std::vector<unsigned> const& subMeshFaces =
				materialFaces[materialIndex];

			size_t const subMeshFaceCount = subMeshFaces.size();

			// РџСЂРѕР±РµРіР°РµРј РїРѕ РјР°СЃСЃРёРІСѓ РіСЂР°РЅРµР№ С‚РµРєСѓС‰РµР№ РїРѕРґСЃРµС‚РєРё
			for (unsigned i = 0; i < subMeshFaceCount; ++i)
			{
				// РёРЅРґРµРєСЃ РіСЂР°РЅРё РїРѕРґСЃРµС‚РєРё
				unsigned faceIndex = subMeshFaces[i];

				MeshFace const& inputFace = faces[faceIndex];

				// РџСЂРѕР±РµРіР°РµРј РїРѕ РёРЅРґРµРєСЃР°Рј С‚РµРєСѓС‰РµР№ РіСЂР°РЅРё
				for (unsigned j = 0; j < 3; ++j)
				{
					// РџРѕР»СѓС‡Р°РµРј РёРЅРґРµРєСЃ С‚РµРєСѓС‰РµР№ РІРµСЂС€РёРЅС‹ РіСЂР°РЅРё
					unsigned vertexIndex = inputFace.vertices[j];

					// РїСЂРѕРІРµСЂСЏРµРј, С‡С‚Рѕ РёРЅРґРµРєСЃ РІРµСЂС€РёРЅС‹ РјРѕР¶РµС‚ Р±С‹С‚СЊ РїСЂРµРґСЃС‚Р°РІР»РµРЅС‹
					// РїСЂРё РїРѕРјРѕС‰Рё С‚РёРїР° IndexType
					if (vertexIndex > (unsigned)maxIndexValue)
					{
						throw std::logic_error("Vertex index is out of range");
					}

					// РІСЃРµ РЅРѕСЂРјР°Р»СЊРЅРѕ, СЃРѕС…СЂР°РЅСЏРµРј РёРЅРґРµРєСЃ РІРµСЂС€РёРЅС‹ РІ РјР°СЃСЃРёРІ РёРЅРґРµРєСЃРѕРІ
					// Рё РїРµСЂРµС…РѕРґРёРј Рє СЃР»РµРґСѓСЋС‰РµР№ РІРµСЂС€РёРЅРµ
					*pOutputIndex++ = static_cast<IndexType>(vertexIndex);
				}
			}
		}

		// Р’РѕР·РІСЂР°С‰Р°РµРј СЃРјРµС‰РµРЅРёРµ РІ РјР°СЃСЃРёРІРµ РёРЅРґРµРєСЃРѕРІ, РїРѕ РєРѕС‚РѕСЂРѕРјСѓ СЂР°СЃРїРѕР»Р°РіР°СЋС‚СЃСЏ
		// РёРЅРґРµРєСЃС‹ РґР°РЅРЅРѕР№ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		return indexBufferOffset;
	}

	// Р Р°СЃС‰РµРїР»СЏРµРј РІРµСЂС€РёРЅС‹ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё СЃ СѓС‡РµС‚РѕРј
	// РіСЂСѓРїРї СЃРіР»Р°Р¶РёРІР°РЅРёСЏ РµРµ РіСЂР°РЅРµР№ Рё С„РѕСЂРјРёСЂСѓРµРј РЅРѕРІС‹Р№ РјР°СЃСЃРёРІ
	// СЃ РёРЅС„РѕСЂРјР°С†РёРµР№ Рѕ РІРµСЂС€РёРЅР°С…, Р° С‚Р°РєР¶Рµ
	// РѕР±РЅРѕРІР»РµРЅРЅСѓСЋ РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ РіСЂР°РЅСЏС… СЃРµС‚РєРё РїРѕСЃР»Рµ 
	// СЂР°СЃС‰РµРїР»РµРЅРёСЏ
	static void SplitVerticesBySmoothGroup(
		Lib3dsMesh const& mesh,
		std::vector<VertexInfo>& outputVertices,
		std::vector<MeshFace>& outputFaces
	)
	{
		// РњР°СЃСЃРёРІ РґР»СЏ С…СЂР°РЅРµРЅРёСЏ РЅРѕСЂРјР°Р»РµР№ РІРµСЂС€РёРЅ РІСЃРµС… РіСЂР°РЅРµР№ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		std::vector<float> faceVertexNormals(mesh.nfaces * 3 * 3);

		// Р¤СѓРЅРєС†РёСЏ lib3ds_mesh_calculate_vertex_normals РІС‹С‡РёСЃР»СЏРµС‚ РЅРѕСЂРјР°Р»СЊ РєР°Р¶РґРѕР№ 
		// РІРµСЂС€РёРЅС‹ РєР°Р¶РґРѕР№ С‚СЂРµСѓРіРѕР»СЊРЅРѕР№ РіСЂР°РЅРё СЃРµС‚РєРё СЃ СѓС‡РµС‚РѕРј РіСЂСѓРїРї СЃРіР»Р°Р¶РёРІР°РЅРёСЏ РіСЂР°РЅРё 
		// Рё РїСЂРёР»РµРіР°СЋС‰РёС… Рє РЅРµР№ РіСЂР°РЅРµР№
		// Р”Р»СЏ РµРµ РІС‹Р·РѕРІР° РЅР°Рј РїСЂРёС€Р»РѕСЃСЊ РґРѕР±Р°РІРёС‚СЊ РЅРµРјРЅРѕРіРѕ "С‡РµСЂРЅРѕР№ РјР°РіРёРё":
		//	Р°) Р’ РєР°С‡РµСЃС‚РІРµ РїРµСЂРІРѕРіРѕ РїР°СЂР°РјРµС‚СЂР° С„СѓРЅРєС†РёСЏ РїСЂРёРЅРёРјР°РµС‚ СѓРєР°Р·Р°С‚РµР»СЊ Lib3dsMesh*
		//		РІРјРµСЃС‚Рѕ Lib3dsMesh const*, С…РѕС‚СЏ РЅРµ РјРѕРґРёС„РёС†РёСЂСѓРµС‚ РїРµСЂРµРґР°РЅРЅСѓСЋ
		//		РїРѕР»РёРіРѕРЅР°Р»СЊРЅСѓСЋ СЃРµС‚РєСѓ. РЎСѓРґСЏ РїРѕ РІСЃРµРјСѓ, СЌС‚Рѕ РЅРµРґРѕСЂР°Р±РѕС‚РєР° Р°РІС‚РѕСЂР° Р±РёР±Р»РёРѕС‚РµРєРё.
		//		РџСЂРёРґРµС‚СЃСЏ СЃРЅСЏС‚СЊ РєРѕРЅСЃС‚Р°РЅС‚РЅРѕСЃС‚СЊ СЃ СѓРєР°Р·Р°С‚РµР»СЏ СЃ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёРµРј const_cast
		//	Р±) Р’ РєР°С‡РµСЃС‚РІРµ РІС‚РѕСЂРѕРіРѕ РїР°СЂР°РјРµС‚СЂР° С„СѓРЅРєС†РёСЏ РїСЂРёРЅРёРјР°РµС‚ СѓРєР°Р·Р°С‚РµР»СЊ
		//		РЅР° РїРµСЂРІС‹Р№ СЌР»РµРјРµРЅС‚ РјР°СЃСЃРёРІР° С‚СЂРµС…РєРѕРјРїРѕРЅРµРЅС‚РЅС‹С… РјР°СЃСЃРёРІРѕРІ 
		//		float-РѕРІ (float[][3]).
		//		РџРѕСЃРєРѕР»СЊРєСѓ РєРѕРЅС‚РµР№РЅРµСЂ faceVertexNormals С…СЂР°РЅРёС‚ СЌР»РµРјРµРЅС‚С‹ С‚РёРїР° float, РґР»СЏ 
		//		РїСЂРµРѕР±СЂР°Р·РѕРІР°РЅРёСЏ СѓРєР°Р·Р°С‚РµР»СЏ float* Рє РЅСѓР¶РЅРѕРјСѓ С‚РёРїСѓ float(*)[3] 
		//		РІРѕСЃРїРѕР»СЊР·СѓРµРјСЃСЏ РѕРїРµСЂР°С‚РѕСЂРѕРј reinterpret_cast
		float (*pCalculatedNormals)[3] =
			reinterpret_cast<float(*)[3]>(&faceVertexNormals[0]);
		// Р’С‹РїРѕР»РЅСЏРµРј РІС‹С‡РёСЃР»РµРЅРёРµ РЅРѕСЂРјР°Р»РµР№ РІ РІРµСЂС€РёРЅР°С… РїСЂРё РїРѕРјРѕС‰Рё lib3ds
		lib3ds_mesh_calculate_vertex_normals(const_cast<Lib3dsMesh*>(&mesh),
			pCalculatedNormals);

		// СЂРµР·РµСЂРІРёСЂСѓРµРј РїР°РјСЏС‚СЊ РїРѕРґ РјР°РєСЃРёРјР°Р»СЊРЅРѕРµ РІРѕР·РјРѕР¶РЅРѕРµ РєРѕР»РёС‡РµСЃС‚РІРѕ РІРµСЂС€РёРЅ
		// РІ РґР°РЅРЅРѕР№ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРµ(РєРѕР»РёС‡РµСЃС‚РІРѕ РіСЂР°РЅРµР№ * 3)
		outputVertices.clear();
		outputVertices.reserve(mesh.nfaces * 3);
		// РњР°СЃСЃРёРІ РёР·РЅР°С‡Р°Р»СЊРЅРѕ Р±СѓРґРµС‚ Р·Р°РїРѕР»РЅРµРЅ РёРЅС„РѕСЂРјР°С†РёРµР№ РѕР± РѕСЂРёРіРёРЅР°Р»СЊРЅС‹С… РІРµСЂС€РёРЅР°С…
		// (РјРµС‚РѕРґ std::vector::resize РІС‹РїРѕР»РЅРёС‚ РёРЅРёС†РёР°Р»РёР·Р°С†РёСЋ РґРѕР±Р°РІР»РµРЅРЅС‹С… РІ РјР°СЃСЃРёРІ
		// СЌР»РµРјРµРЅС‚РѕРІ СЃ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёРµРј РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂР° РїРѕ СѓРјРѕР»С‡Р°РЅРёСЋ, РєРѕС‚РѕСЂС‹Р№
		// РёРЅРёС†РёР°Р»РёР·РёСЂСѓРµС‚ РІРµСЂС€РёРЅСѓ РєР°Рє РѕСЂРёРіРёРЅР°Р»СЊРЅСѓСЋ)
		outputVertices.resize(mesh.nvertices);

		// РћС‡РёС‰Р°РµРј РјР°СЃСЃРёРІ РІС‹С…РѕРґРЅС‹С… РІРµСЂС€РёРЅ (РЅР° СЃР»СѓС‡Р°Р№, РµСЃР»Рё РѕРЅ РѕРєР°Р¶РµС‚СЃСЏ РЅРµРїСѓСЃС‚С‹Рј)
		outputFaces.clear();
		// Р—Р°СЂР°РЅРµРµ СЂРµР·РµСЂРІРёСЂСѓРµРј РІ РЅРµРј РјРµСЃС‚Рѕ РґР»СЏ С…СЂР°РЅРµРЅРёСЏ РІСЃРµС… РіСЂР°РЅРµР№ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ 
		// СЃРµС‚РєРё, С‡С‚РѕР±С‹ РґРѕР±Р°РІР»РµРЅРёРµ РІ РЅРµРіРѕ РЅРѕРІС‹С… СЌР»РµРјРµРЅС‚РѕРІ РЅРµ РїСЂРёРІРѕРґРёР»Рѕ Рє РїРѕРІС‚РѕСЂРЅРѕРјСѓ
		// РІС‹РґРµР»РµРЅРёСЋ РїР°РјСЏС‚Рё РІ РєСѓС‡Рµ (РЅРµР±РѕР»СЊС€Р°СЏ РѕРїС‚РёРјРёР·Р°С†РёСЏ)
		outputFaces.reserve(mesh.nfaces);

		// РРЅРґРµРєСЃ РЅРѕСЂРјР°Р»Рё РІ РјР°СЃСЃРёРІРµ РІС‹С‡РёСЃР»РµРЅРЅС‹С… РїСЂРё РїРѕРјРѕС‰Рё lib3ds РЅРѕСЂРјР°Р»РµР№
		size_t calculatedNormalIndex = 0;
		// Р’ С†РёРєР»Рµ Р±СѓРґСѓС‚ РїРѕСЃР»РµРґРѕРІР°С‚РµР»СЊРЅРѕ РѕР±СЂР°Р±РѕС‚Р°РЅС‹ РІСЃРµ РіСЂР°РЅРё СЃРµС‚РєРё
		for (size_t faceIndex = 0; faceIndex < mesh.nfaces; ++faceIndex)
		{
			// РЎСЃС‹Р»РєР° РЅР° С‚РµРєСѓС‰СѓСЋ РіСЂР°РЅСЊ
			Lib3dsFace const& face = mesh.faces[faceIndex];

			// Р’С‹С…РѕРґРЅР°СЏ РіСЂР°РЅСЊ РґР»СЏ РїРѕРјРµС‰РµРЅРёСЏ РІ РјР°СЃСЃРёРІ outputFaces, РєРѕС‚РѕСЂР°СЏ Р±СѓРґРµС‚ 
			// Р·Р°РїРѕР»РЅРµРЅР° РёРЅРґРµРєСЃР°РјРё РІРµСЂС€РёРЅ (СЃ РёР·РјРµРЅРµРЅРёРµРј РѕСЂРёРіРёРЅР°Р»СЊРЅРѕР№ РЅСѓРјРµСЂР°С†РёРё 
			// РїСЂРё РґРѕР±Р°РІР»РµРЅРёРё РїРѕСЂРѕР¶РґРµРЅРЅС‹С… РІРµСЂС€РёРЅ)
			MeshFace outputFace;
			// Р—Р°РґР°РµРј РёРЅРґРµРєСЃ РјР°С‚РµСЂРёР°Р»Р°
			outputFace.materialIndex = face.material;

			// С†РёРєР» РїРѕ РёРЅРґРµРєСЃР°Рј РІРµСЂС€РёРЅ С‚РµРєСѓС‰РµР№ РіСЂР°РЅРё
			for (unsigned i = 0; i < 3; ++i)
			{
				// РџРѕР»СѓС‡Р°РµРј РІС‹С‡РёСЃР»РµРЅРЅС‹Р№ РІРµРєС‚РѕСЂ РЅРѕСЂРјР°Р»Рё С‚РµРєСѓС‰РµР№ РІРµСЂС€РёРЅС‹
				// РёР· РјР°СЃСЃРёРІР° РІС‹С‡РёСЃР»РµРЅРЅС‹С… РЅРѕСЂРјР°Р»РµР№, СѓРІРµР»РёС‡РёРІР°СЏ РёРЅРґРµРєСЃ РЅРѕСЂРјР°Р»Рё
				glm::dvec3 calculatedNormal(
					pCalculatedNormals[calculatedNormalIndex][0],
					pCalculatedNormals[calculatedNormalIndex][1],
					pCalculatedNormals[calculatedNormalIndex][2]
				);
				calculatedNormalIndex++;

				// РџРѕР»СѓС‡Р°РµРј РёСЃС…РѕРґРЅС‹Р№ РёРЅРґРµРєСЃ С‚РµРєСѓС‰РµР№ РІРµСЂС€РёРЅС‹ РѕР±СЂР°Р±Р°С‚С‹РІР°РµРјРѕР№ РіСЂР°РЅРё
				unsigned vertexIndex = face.index[i];

				// РџСЂРѕРІРµСЂСЏРµРј, РЅРµ РІС‹С…РѕРґРёС‚ Р»Рё РёРЅРґРµРєСЃ РІРµСЂС€РёРЅС‹ Р·Р° РїСЂРµРґРµР»С‹ РјР°СЃСЃРёРІР° РІРµСЂС€РёРЅ.
				// Р’ РЅРѕСЂРјР°Р»СЊРЅРѕР№ СЃРёС‚СѓР°С†РёРё С‚Р°РєРѕРµ РЅРµРІРѕР·РјРѕР¶РЅРѕ, С…РѕС‚СЏ РїСЂРё РїРѕРІСЂРµР¶РґРµРЅРёРё
				// РІС…РѕРґРЅРѕРіРѕ С„Р°Р№Р»Р° РЅРµ РёСЃРєР»СЋС‡РµРЅРѕ
				if (vertexIndex >= mesh.nvertices)
				{
					throw std::runtime_error("Vertex index is out of vertex array");
				}

				// Р¦РёРєР» РїРѕРёСЃРєР° РІРµСЂС€РёРЅС‹, РЅРѕСЂРјР°Р»СЊ РєРѕС‚РѕСЂРѕР№ СЃРѕРІРїР°РґР°РµС‚ СЃ РІС‹С‡РёСЃР»РµРЅРЅС‹Рј
				// Р·РЅР°С‡РµРЅРёРµРј РЅРѕСЂРјР°Р»Рё.
				while (true)
				{
					// РџРѕР»СѓС‡Р°РµРј РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ СЃСѓС‰РµСЃС‚РІСѓСЋС‰РµР№ РІРµСЂС€РёРЅРµ
					VertexInfo& vertex = outputVertices[vertexIndex];

					// Р·Р°РґР°РЅР° Р»Рё РґР»СЏ РЅРµРµ РЅРѕСЂРјР°Р»СЊ?
					if (!vertex.NormalIsDefined())
					{
						// РќРѕСЂРјР°Р»СЊ РІ РїСЂРѕРІРµСЂСЏРµРјРѕР№ РІРµСЂС€РёРЅРµ РµС‰Рµ РЅРµ Р·Р°РґР°РЅР°, Р·РЅР°С‡РёС‚, РІ РЅРµРµ
						// РЅСѓР¶РЅРѕ Р·Р°РїРёСЃР°С‚СЊ РІС‹С‡РёСЃР»РµРЅРЅРѕРµ Р·РЅР°С‡РµРЅРёРµ РЅРѕСЂРјР°Р»Рё
						vertex.SetNormal(calculatedNormal);
						// РџРѕРёСЃРє Р·Р°РІРµСЂС€РµРЅ. РРЅРґРµРєСЃ РёСЃРєРѕРјРѕР№ РІРµСЂС€РёРЅС‹ - РІ РїРµСЂРµРјРµРЅРЅРѕР№
						// vertexIndex
						break;
					}
					else // РЅРѕСЂРјР°Р»СЊ РІ РІРµСЂС€РёРЅРµ Р±С‹Р»Р° РѕРїСЂРµРґРµР»РµРЅР°
					{
						// СЃСЂР°РІРЅРёРІР°РµРј РІС‹С‡bСЃР»РµРЅРЅС‹Р№ РІРµРєС‚РѕСЂ РЅРѕСЂРјР°Р»Рё СЃ СЃСѓС‰РµСЃС‚РІСѓСЋС‰РёРј.
						// Рў.Рє. РІС‹С‡РёСЃР»РµРЅРёРµ РЅРѕСЂРјР°Р»РµР№ РїСЂРѕРёСЃС…РѕРґРёС‚ СЃ РЅРµРєРѕС‚РѕСЂРѕР№ РїРѕРіСЂРµС€РЅРѕСЃС‚СЊСЋ,
						// РёСЃРїРѕР»СЊР·СѓРµРј РїСЂРѕРІРµСЂРєСѓ РІРµРєС‚РѕСЂРѕРІ РЅР° РїСЂРёР±Р»РёР·РёС‚РµР»СЊРЅРѕРµ СЂР°РІРµРЅСЃС‚РІРѕ:
						// РёС… СЂР°Р·РЅРѕСЃС‚СЊ РїРѕ Р°Р±СЃРѕР»СЋС‚РЅРѕР№ РІРµР»РёС‡РёРЅРµ РґРѕР»Р¶РЅР° Р±С‹С‚СЊ РјРµРЅСЊС€Рµ 0.00001
						// Р§РёСЃР»Рѕ 0.00001 РІРїРѕР»РЅРµ РїРѕРґС…РѕРґРёС‚ РґР»СЏ СЃСЂР°РІРЅРµРЅРёСЏ РµРґРёРЅРёС‡РЅС‹С… РІРµРєС‚РѕСЂРѕРІ.
						// РСЃРїРѕР»СЊР·РѕРІР°С‚СЊ Р±РћР»СЊС€СѓСЋ С‚РѕС‡РЅРѕСЃС‚СЊ РґР»СЏ С‡РёСЃРµР» С‚РёРїР° float РѕСЃРѕР±РѕРіРѕ
						// СЃРјС‹СЃР»Р° РЅРµ РёРјРµРµС‚
						if (glm::length(calculatedNormal - vertex.GetNormal()) < 1e-5f)
						{
							// РІС‹С…РѕРґРёРј РёР· С†РёРєР»Р° - РїРѕРёСЃРє РѕРєРѕРЅС‡РµРЅ
							// РРЅРґРµРєСЃ РёСЃРєРѕРјРѕР№ РІРµСЂС€РёРЅС‹ РЅР°С…РѕРґРёС‚СЃСЏ РІ РїРµСЂРµРјРµРЅРЅРѕР№ vertexIndex
							break;
						}
						else
						{	// РІС‹С‡РёСЃР»РµРЅРЅР°СЏ РЅРѕСЂРјР°Р»СЊ РѕС‚Р»РёС‡Р°РµС‚СЃСЏ РѕС‚ РЅРѕСЂРјР°Р»Рё РїСЂРѕРІРµСЂСЏРµРјРѕР№
							// РІРµСЂС€РёРЅС‹

							// РџРѕР»СѓС‡Р°РµРј РёРЅРґРµРєСЃ РІРµСЂС€РёРЅС‹, РїРѕСЂРѕР¶РґРµРЅРЅРѕР№ РѕС‚ РґР°РЅРЅРѕР№
							int derivedVertexIndex = vertex.GetDerivedVertex();

							// Р•СЃС‚СЊ Р»Рё РІРѕРѕР±С‰Рµ РїРѕСЂРѕР¶РґРµРЅРЅР°СЏ РІРµСЂС€РёРЅР°?
							if (derivedVertexIndex < 0)
							{	// РџРѕСЂРѕР¶РґРµРЅРЅС‹С… РІРµСЂС€РёРЅ Р±РѕР»СЊС€Рµ РЅРµС‚, Р° С‚.Рє. РІС‹С‡РёСЃР»РµРЅРЅР°СЏ
								// РЅРѕСЂРјР°Р»СЊ РЅР°Р№РґРµРЅР° РЅРµ Р±С‹Р»Р°, РїСЂРёРґРµС‚СЃСЏ РґРѕР±Р°РІРёС‚СЊ
								// РїРѕСЂРѕР¶РґРµРЅРЅСѓСЋ РІРµСЂС€РёРЅСѓ, РїРѕСЂРѕРґРёРІ РµРµ РѕС‚ С‚РµРєСѓС‰РµР№

								// РџРѕСЂРѕР¶РґР°РµРј РЅРѕРІСѓСЋ РІРµСЂС€РёРЅСѓ РѕС‚ С‚РµРєСѓС‰РµР№ РІРµСЂС€РёРЅС‹
								VertexInfo newVertex(calculatedNormal, vertexIndex);

								// РРЅРґРµРєСЃ *РёСЃРєРѕРјРѕР№* РІРµСЂС€РёРЅС‹ Р±СѓРґРµС‚ СЂР°РІРµРЅ РёРЅРґРµРєСЃСѓ
								// СЃРѕР·РґР°РЅРЅРѕР№ РІРµСЂС€РёРЅС‹. Рў.Рє. РІРµСЂС€РёРЅС‹ РґРѕР±Р°РІР»СЏСЋС‚СЃСЏ РІ РєРѕРЅРµС†
								// РјР°СЃСЃРёРІР° РІРµСЂС€РёРЅ, РёРЅРґРµРєСЃ Р±СѓРґРµС‚ СЂР°РІРµРЅ С‚РµРєСѓС‰РµРјСѓ
								// РєРѕР»РёС‡РµСЃС‚РІСѓ РІРµСЂС€РёРЅ РІ РјР°СЃСЃРёРІРµ
								vertexIndex = static_cast<int>(outputVertices.size());

								// Р”Р»СЏ С‚РµРєСѓС‰РµР№ РІРµСЂС€РёРЅС‹ Р·Р°РґР°РµРј РёРЅРґРµРєСЃ РїРѕСЂРѕР¶РґРµРЅРЅРѕР№ 
								// (РЅР°Р№РґРµРЅРЅРѕР№) РІРµСЂС€РёРЅС‹
								vertex.DeriveVertex(vertexIndex);

								// РґРѕР±Р°РІР»СЏРµРј РЅРѕРІСѓСЋ РІРµСЂС€РёРЅСѓ РІ РјР°СЃСЃРёРІ РІРµРєС‚РѕСЂРѕРІ
								outputVertices.push_back(newVertex);

								// РІС‹С…РѕРґРёРј РёР· С†РёРєР»Р° - РїРѕРёСЃРє РѕРєРѕРЅС‡РµРЅ
								// РРЅРґРµРєСЃ РёСЃРєРѕРјРѕР№ РІРµСЂС€РёРЅС‹ РЅР°С…РѕРґРёС‚СЃСЏ РІ РїРµСЂРµРјРµРЅРЅРѕР№
								// vertexIndex
								break;
							}
							else
							{
								// РїРѕСЂРѕР¶РґРµРЅРЅР°СЏ РІРµСЂС€РёРЅР° Сѓ РІРµСЂС€РёРЅС‹ СЃ РёРЅРґРµРєСЃРѕРј vertexIndex
								// РёРјРµРµС‚СЃСЏ. РЎР»РµРґРѕРІР°С‚РµР»СЊРЅРѕ РІС‹С€РµРѕРїРёСЃР°РЅРЅС‹Рµ РґРµР№СЃС‚РІРёСЏ РЅСѓР¶РЅРѕ
								// РїСЂРѕРґРµР»Р°С‚СЊ РЅР°Рґ РїРѕСЂРѕР¶РґРµРЅРЅРѕР№ РІРµСЂС€РёРЅРѕР№ РЅР° СЃР»РµРґСѓСЋС‰РµР№ 
								// РёС‚РµСЂР°С†РёРё РґР°РЅРЅРѕРіРѕ С†РёРєР»Р°, РїРѕРєР° РІРµСЂС€РёРЅР° СЃ РІС‹С‡РёСЃР»РµРЅРЅС‹Рј
								// Р·РЅР°С‡РµРЅРёРµРј РЅРѕСЂРјР°Р»Рё РЅРµ Р±СѓРґРµС‚ РЅР°Р№РґРµРЅР° СЃСЂРµРґРё С†РµРїРѕС‡РєРё
								// РїРѕСЂРѕР¶РґРµРЅРЅС‹С… РІРµСЂС€РёРЅ, Р»РёР±Рѕ РїРѕРєР° С†РµРїРѕС‡РєР° РЅРµ Р·Р°РєРѕРЅС‡РёС‚СЃСЏ
								vertexIndex = derivedVertexIndex;

								// РїРµСЂРµС…РѕРґРёРј Рє СЃР»РµРґСѓСЋС‰РµР№ РёС‚РµСЂР°С†РёРё С†РёРєР»Р° РїРѕРёСЃРєР° РІРµСЂС€РёРЅС‹ 
								// СЃ СЃРѕРІРїР°РґР°СЋС‰РµРј РІРµРєС‚РѕСЂРѕРј РЅРѕСЂРјР°Р»Рё
							}
						}
					}
				}	// while (true)

				// РџСЂРё РІС‹С…РѕРґРµ РёР· РїСЂРµРґС‹РґСѓС‰РµРіРѕ С†РёРєР»Р° while РІ РїРµСЂРµРјРµРЅРЅРѕР№ vertexIndex 
				// Р±СѓРґРµС‚ РЅР°С…РѕРґРёС‚СЊСЃСЏ РёРЅРґРµРєСЃ РІРµСЂС€РёРЅС‹ (Р»РёР±Рѕ СЃСѓС‰РµСЃС‚РІСѓСЋС‰РµР№, Р»РёР±Рѕ
				// РїРѕСЂРѕР¶РґРµРЅРЅРѕР№)
				outputFace.vertices[i] = vertexIndex;

			}	// for (unsigned i = 0; i < 3; ++i)

			// РґРѕР±Р°РІР»СЏРµРј РіСЂР°РЅСЊ РІ РјР°СЃСЃРёРІ РіСЂР°РЅРµР№
			outputFaces.push_back(outputFace);
		}	// for (size_t faceIndex = 0; faceIndex < mesh.nfaces; ++faceIndex)
	}

	// Р—Р°РЅРѕСЃРёС‚ РІ РІРµСЂС€РёРЅРЅС‹Р№ Р±СѓС„РµСЂР° РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ РЅРѕСЂРјР°Р»СЏС…,
	// Р° С‚Р°РєР¶Рµ Рѕ РґРѕРїРѕР»РЅРёС‚РµР»СЊРЅС‹С… РІРµСЂС€РёРЅР°С…, РїРѕР»СѓС‡РёРІС€РёС…СЃСЏ РІ СЂРµР·СѓР»СЊС‚Р°С‚Рµ
	// СЂР°СЃС‰РµРїР»РµРЅРёСЏ РѕСЂРёРіРёРЅР°Р»СЊРЅС‹С… РІРµСЂС€РёРЅ СЃ СѓС‡РµС‚РѕРј РіСЂСѓРїРї СЃРіР»Р°Р¶РёРІР°РЅРёСЏ
	// РџР°СЂР°РјРµС‚СЂС‹:
	//	mesh - РѕСЂРёРіРёРЅР°Р»СЊРЅР°СЏ СЃРµС‚РєР°
	//	vertexBufferData - РјР°СЃСЃРёРІ, СЃРѕРґРµСЂР¶Р°С‰РёР№ РґР°РЅРЅС‹Рµ РґР»СЏ Р±СѓС„РµСЂР° РІРµСЂС€РёРЅ
	//	vertexBufferOffset - СЃРјРµС‰РµРЅРёРµ РІ РјР°СЃСЃРёРІРµ Рє РЅР°С‡Р°Р»Сѓ РёРЅС„РѕСЂРјР°С†РёРё 
	//		Рѕ РІРµСЂС€РёРЅР°С… РґР°РЅРЅРѕР№ РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
	//	outputFaces - РІС‹С…РѕРґРЅРѕР№ РјР°СЃСЃРёРІ СЃ РѕР±РЅРѕРІР»РµРЅРЅРѕР№ РёРЅС„РѕСЂРјР°С†РёРµР№
	//		Рѕ РіСЂР°РЅСЏС…
	//	Р’РѕР·РІСЂР°С‰Р°РµРјРѕРµ Р·РЅР°С‡РµРЅРёРµ:
	//		РєРѕР»РёС‡РµСЃС‚РІРѕ РІРµСЂС€РёРЅ РІ СЂРµР·СѓР»СЊС‚РёСЂСѓСЋС‰РµР№ СЃРµС‚РєРµ
	template <class VertexType>
	static unsigned SplitVerticesAndBuildNormals(
		Lib3dsMesh const& mesh,
		std::vector<unsigned char>& vertexBufferData,
		size_t vertexBufferOffset,
		std::vector<MeshFace>& outputFaces
	)
	{
		// РЎРѕР·РґР°РµРј РјР°СЃСЃРёРІ РґР»СЏ С…СЂР°РЅРµРЅРёСЏ РёРЅС„РѕСЂРјР°С†РёРµРё Рѕ РІРµСЂС€РёРЅР°С…
	// РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		std::vector<VertexInfo> vertices;

		// Р’С‹С‡РёСЃР»СЏРµРј РЅРѕСЂРјР°Р»Рё Рє РІРµСЂС€РёРЅР°Рј РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё,
		// РїСЂРё РЅРµРѕР±С…РѕРґРёРјРѕСЃС‚Рё РІС‹РїРѕР»РЅСЏСЏ РїРѕСЂРѕР¶РґРµРЅРёРµ РЅРѕРІС‹С… РІРµСЂС€РёРЅ
		// СЃ РѕР±РЅРѕРІР»РµРЅРёРµРј РёРЅРґРµРєСЃРѕРІ РіСЂР°РЅРµР№
		SplitVerticesBySmoothGroup(mesh, vertices, outputFaces);

		size_t const numberOfVertices = vertices.size();

		// РћР±РЅРѕРІР»СЏРµРј СЂР°Р·РјРµСЂ РјР°СЃСЃРёРІР° РІРµСЂС€РёРЅ, РїСЂРёРЅРёРјР°СЏ РІРѕ РІРЅРёРјР°РЅРёРµ РєРѕР»РёС‡РµСЃС‚РІРѕ
		// РІРµСЂС€РёРЅ, РїРѕР»СѓС‡РёРІС€РµРµСЃСЏ РїРѕСЃР»Рµ СЂР°СЃС‰РµРїР»РµРЅРёСЏ РІРµСЂС€РёРЅ
		vertexBufferData.resize(
			vertexBufferOffset + sizeof(VertexType) * numberOfVertices);

		// РІС‹С‡РёСЃР»СЏРµРј Р°РґСЂРµСЃ СЂР°СЃРїРѕР»РѕР¶РµРЅРёСЏ РІРµСЂС€РёРЅРЅС‹С… РґР°РЅРЅС‹С… РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		// РІ Р±СѓС„РµСЂРµ
		VertexType* outputVertices =
			reinterpret_cast<VertexType*>(&vertexBufferData[vertexBufferOffset]);

		// Р¦РёРєР» РїРѕ СЂРµР·СѓР»СЊС‚РёСЂСѓСЋС‰РёРј РІРµСЂС€РёРЅР°Рј, РІ РєРѕС‚РѕСЂРѕРј Р·РЅР°С‡РµРЅРёРµ РґР°РЅРЅС‹Рµ Рѕ
		// РїРѕСЂРѕР¶РґРµРЅРЅС‹С… РІРµСЂС€РёРЅР°С…, Р° С‚Р°РєР¶Рµ РІС‹С‡РёСЃР»РµРЅРЅС‹Рµ РЅРѕСЂРјР°Р»Рё Р±СѓРґСѓС‚
		// СЃРєРѕРїРёСЂРѕРІР°РЅС‹ РІ РІС‹С…РѕРґРЅРѕР№ Р±СѓС„РµСЂ
		for (size_t vertexIndex = 0; vertexIndex < numberOfVertices; ++vertexIndex)
		{
			// РёРЅС„РѕСЂРјР°С†РёСЏ Рѕ С‚РµРєСѓС‰РµР№ РІРµСЂС€РёРЅРµ
			VertexInfo const& vertexInfo = vertices[vertexIndex];
			// СЃСЃС‹Р»РєР° РЅР° РґР°РЅРЅС‹Рµ РѕР± СЌС‚РѕР№ РІРµСЂС€РёРЅРµ РІ РІРµСЂС€РёРЅРЅРѕРј Р±СѓС„РµСЂРµ
			VertexType& outputVertex = outputVertices[vertexIndex];

			// Р•СЃР»Рё РІРµСЂС€РёРЅР° СЏРІР»СЏРµС‚СЃСЏ РїРѕСЂРѕР¶РґРµРЅРЅРѕР№, С‚Рѕ РєРѕРїРёСЂСѓРµРј РІ РЅРµРµ РґР°РЅРЅС‹Рµ РёР· 
			// РїРѕСЂРѕРґРёРІС€РµР№ РµРµ РІРµСЂС€РёРЅС‹
			int originalVertexIndex = vertexInfo.GetOriginalVertexIndex();
			if (originalVertexIndex >= 0)
			{
				// СѓРґРѕСЃС‚РѕРІРµСЂСЏРµРјСЃСЏ (РІ РѕС‚Р»Р°РґРѕС‡РЅРѕР№ РєРѕРЅС„РёРіСѓСЂР°С†РёРё), С‡С‚Рѕ РґР°РЅРЅС‹Рµ РєРѕРїРёСЂСѓСЋС‚СЃСЏ
				// РёР· СЂР°РЅРµРµ РѕР±СЂР°Р±РѕС‚Р°РЅРЅРѕР№ РІРµСЂС€РёРЅС‹
				assert(static_cast<unsigned>(originalVertexIndex) < vertexIndex);
				// СЃСЃС‹Р»РєР° РЅР° РІРµСЂС€РёРЅСѓ, РїРѕСЂРѕРґРёРІС€СѓСЋ РґР°РЅРЅСѓСЋ
				VertexType const& originalVertex =
					outputVertices[originalVertexIndex];
				// РєРѕРїРёСЂСѓРµРј РїРѕСЂРѕР¶РґР°СЋС‰СѓСЋ РІРµСЂС€РёРЅСѓ РІ С‚РµРєСѓС‰СѓСЋ
				outputVertex = originalVertex;
			}

			// Р•СЃР»Рё Сѓ РІРµСЂС€РёРЅС‹ Р±С‹Р» РѕРїСЂРµРґРµР»РµРЅ РІРµРєС‚РѕСЂ РЅРѕСЂРјР°Р»Рё, С‚Рѕ РєРѕРїРёСЂСѓРµРј РµРіРѕ РІ
			// РІРµСЂС€РёРЅРЅС‹Р№ Р±СѓС„РµСЂ
			// РЈ РІРµСЂС€РёРЅС‹ РІРµРєС‚РѕСЂ РЅРѕСЂРјР°Р»Рё Р·Р°РґР°РЅ, РµСЃР»Рё РІРµСЂС€РёРЅР° РІС…РѕРґРёР»Р° РІ СЃРѕСЃС‚Р°РІ
			// С…РѕС‚СЏ Р±С‹ РѕРґРЅРѕР№ РіСЂР°РЅРё СЃРµС‚РєРё
			if (vertexInfo.NormalIsDefined())
			{
				glm::dvec3 const& normal = vertexInfo.GetNormal();
				outputVertex.normal.x = normal.x;
				outputVertex.normal.y = normal.y;
				outputVertex.normal.z = normal.z;
			}
		}

		// Р’РѕР·РІСЂР°С‰Р°РµРј РєРѕР»РёС‡РµСЃС‚РІРѕ РІРµСЂС€РёРЅ, РїРѕР»СѓС‡РµРЅРЅС‹С… РїРѕСЃР»Рµ СЂР°СЃС‰РµРїР»РµРЅРёСЏ
		return static_cast<unsigned>(numberOfVertices);
	}

	// Р’С‹РїРѕР»РЅСЏРµРј РїРѕСЃС‚СЂРѕРµРЅРёРµ СЃРїРёСЃРєРѕРІ РіСЂР°РЅРµР№, РёСЃРїРѕР»СЊР·СѓСЋС‰РёС…
	// СЃРіСЂСѓРїРїРёСЂРѕРІР°РЅРЅС‹С… РїРѕ РёРЅРґРµРєСЃСѓ РјР°С‚РµСЂРёР°Р»Р°
	static void BuildMaterialFacesList(
		unsigned materialCount,
		std::vector<MeshFace> const& faces,
		std::vector< std::vector<unsigned> >& materialFaces)
	{
		// РѕР±РЅСѓР»СЏРµРј СЃС‡РµС‚С‡РёРєРё РіСЂР°РЅРµР№ РєР°Р¶РґРѕРіРѕ РјР°С‚РµСЂРёР°Р»Р° 
	// (РІРєР»СЋС‡Р°СЏ РјР°С‚РµСЂРёР°Р» СЃ РёРЅРґРµРєСЃРѕРј -1, РёСЃРїРѕР»СЊР·СѓРµРјС‹Р№
	// РґР»СЏ РіСЂР°РЅРµР№, Р±РµР· РјР°С‚РµСЂРёР°Р»Р°)
		std::vector<unsigned> materialFaceCount(materialCount + 1);

		unsigned const faceCount = static_cast<unsigned>(faces.size());

		// РџСЂРѕР±РµРіР°РµРј РїРѕ РІСЃРµРј РіСЂР°РЅСЏРј РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		for (size_t faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			// РёРЅРґРµРєСЃ РјР°С‚РµСЂРёР°Р»Р° РіСЂР°РЅРё
			int faceMaterial = faces[faceIndex].materialIndex;

			if (faceMaterial < -1 ||
				faceMaterial >= static_cast<int>(materialCount))
			{
				// РЅРµРєРѕСЂСЂРµРєС‚С‹Р№ РёРЅРґРµРєСЃ РјР°С‚РµСЂРёР°Р»Р°
				throw std::out_of_range(
					"Face material index is out of range");
			}

			// РЈРІРµР»РёС‡РёРІР°РµРј СЃС‡РµС‚С‡РёРє РіСЂР°РЅРµР№, РёСЃРїРѕР»СЊР·СѓСЋС‰РёС… РґР°РЅРЅС‹Р№ РјР°С‚РµСЂРёР°Р»
			// (0 РёРЅРґРµРєСЃ СЃРѕРѕС‚РІРµС‚СЃС‚РІСѓРµС‚ РјР°С‚РµСЂРёР°Р»Сѓ=-1)
			++materialFaceCount[faceMaterial + 1];
		}

		// РљР°Р¶РґРѕРјСѓ РјР°С‚РµСЂРёР°Р»Сѓ СЃРµС‚РєРё РёР·РЅР°С‡Р°Р»СЊРЅРѕ Р±СѓРґРµС‚ СЃРѕРѕС‚РІРµС‚СЃС‚РІРѕРІР°С‚СЊ 
		// РїСѓСЃС‚РѕР№ РјР°СЃСЃРёРІ РёРЅРґРµРєСЃРѕРІ РіСЂР°РЅРµР№
		materialFaces.assign(materialCount + 1, std::vector<unsigned>());

		// Р РµР·РµСЂРІРёСЂСѓРµРј РјРµСЃС‚Рѕ РІ РјР°СЃСЃРёРІР°С… РёРЅРґРµРєСЃРѕРІ РіСЂР°РЅРµР№ РґР»СЏ С…СЂР°РЅРµРЅРёСЏ
		// С‚СЂРµР±СѓРµРјРѕРіРѕ РєРѕР»РёС‡РµСЃС‚РІР° РіСЂР°РЅРµР№ (С‡С‚РѕР±С‹ РёР·Р±РµР¶Р°С‚СЊ РїРµСЂРµРІС‹РґРµР»РµРЅРёСЏ РїР°РјСЏС‚Рё)
		// РїСЂРё СЂРѕСЃС‚Рµ СЂР°Р·РјРµСЂР° РјР°СЃСЃРёРІРѕРІ
		// Р¦РёРєР» РѕС‚ 0 РґРѕ materialCount (РІРєР»СЋС‡РёС‚РµР»СЊРЅРѕ, РґР»СЏ РѕР±СЂР°Р±РѕС‚РєРё РјР°С‚РµСЂРёР°Р»Р°
		// СЃ РёРЅРґРµРєСЃРѕРј СЂР°РІРЅС‹Рј -1)
		for (size_t materialIndex = 0;
			materialIndex <= materialCount;
			++materialIndex)
		{
			std::vector<unsigned>& faces = materialFaces[materialIndex];
			faces.reserve(materialFaceCount[materialIndex]);
		}

		// РїСЂРѕР±РµРіР°РµРј РїРѕ РіСЂР°РЅСЏРј СЃРµС‚РєРё, РґРѕР±Р°РІР»СЏСЏ РёРЅРґРµРєСЃ РіСЂР°РЅРё
		// РІ РјР°СЃСЃРёРІ РіСЂР°РЅРµР№, РёСЃРїРѕР»СЊР·СѓСЋС‰РёС… СЃРѕРѕС‚РІРµС‚СЃС‚РІСѓСЋС‰РёР№ РјР°С‚РµСЂРёР°Р»
		for (unsigned faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			// РёРЅРґРµРєСЃ РјР°С‚РµСЂРёР°Р»Р° РіСЂР°РЅРё
			int faceMaterial = faces[faceIndex].materialIndex;

			assert(static_cast<unsigned>(faceMaterial) + 1 <= materialCount);

			// СЃСЃС‹Р»РєР° РЅР° РјР°СЃСЃРёРІ РіСЂР°РЅРµР№, РёСЃРїРѕР»СЊР·СѓСЋС‰РёС… РґР°РЅРЅС‹Р№ РјР°С‚РµСЂРёР°Р»
			std::vector<unsigned>& faces = materialFaces[faceMaterial + 1];

			// Р·Р°РЅРѕСЃРёРј РІ РїРѕР»СѓС‡РµРЅРЅС‹Р№ РјР°СЃСЃРёРІ РіСЂР°РЅРµР№ РёРЅРґРµРєСЃ С‚РµРєСѓС‰РµР№ РіСЂР°РЅРё
			faces.push_back(faceIndex);
		}
	}

private:
	GLenum m_vertexBufferUsage;
	GLenum m_indexBufferUsage;

	bool m_ignoreMissingTextures;
};
