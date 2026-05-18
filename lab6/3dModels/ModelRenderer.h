#pragma once
#include "pch.h"
#include "Model.h"
#include "Mesh.h"
#include "Vertex.h"

class ModelRenderer
{
public:
	ModelRenderer()
		:m_texCoordsEnabled(false)
		, m_texture2DEnabled(false)
		, m_cullFace(false)
	{
	}

	void RenderModel(const Model& model) const
	{
		const unsigned meshCount = static_cast<unsigned>(model.GetMeshCount());

		if (meshCount == 0)
		{
			return;
		}

		model.GetVertexBuffer().Bind();
		model.GetIndexBuffer().Bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glDisable(GL_TEXTURE_2D);

		m_texCoordsEnabled = !model.GetMesh(0).HasTextureCoords();
		m_texture2DEnabled = false;

		m_cullFace = true;

		const int materialCount = static_cast<int>(model.GetMeterialCount());

		// Transparent materials go in a second pass, otherwise glass writes
		// into the depth buffer too early and hides opaque geometry behind it.
		for (int pass = 0; pass < 2; ++pass)
		{
			const bool transparentPass = (pass == 1);
			glDepthMask(transparentPass ? GL_FALSE : GL_TRUE);

			for (int material = -1; material < materialCount; ++material)
			{
				if (!ShouldRenderMaterialInPass(model, material, transparentPass))
				{
					continue;
				}

				bool materialActivated = false;

				for (unsigned mesh = 0; mesh < meshCount; ++mesh)
				{
					bool needToActivateMaterial =
						!materialActivated && material >= 0;

					materialActivated |= RenderMaterialSubMesh(
						model, mesh, material, needToActivateMaterial);
				}
			}
		}

		glDepthMask(GL_TRUE);

		if (!m_cullFace)
		{
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
			glEnable(GL_CULL_FACE);
		}

		if (m_texture2DEnabled)
		{
			glDisable(GL_TEXTURE_2D);
			m_texture2DEnabled = false;
		}

		if (m_texCoordsEnabled)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

private:
	bool ShouldRenderMaterialInPass(
		Model const& model,
		int materialIndex,
		bool transparentPass
	) const
	{
		if (materialIndex < 0)
		{
			return !transparentPass;
		}

		bool const transparent = model.GetMaterial(materialIndex).IsTransparent();
		return transparentPass ? transparent : !transparent;
	}

	bool RenderMaterialSubMesh(
		Model const& model,	// РјРѕРґРµР»СЊ
		unsigned meshIndex,		// РёРЅРґРµРєСЃ СЃРµС‚РєРё
		int materialIndex,		// РёРЅРґРµРєСЃ РјР°С‚РµСЂРёР°Р»Р°
		bool activateMaterial	// РЅСѓР¶РЅРѕ Р»Рё Р°РєС‚РёРІРёСЂРѕРІР°С‚СЊ РјР°С‚РµСЂРёР°Р»?
	)const
	{
		// РџРѕР»СѓС‡Р°РµРј СЃРµС‚РєСѓ РїРѕ РµРµ РёРЅРґРµРєСЃСѓ
		Mesh const& mesh = model.GetMesh(meshIndex);

		// РџРѕР»СѓС‡Р°РµРј РёРЅРґРµРєСЃ РїРѕРґСЃРµС‚РєРё, РёСЃРїРѕР»СЊР·СѓСЋС‰РµР№ РјР°С‚РµСЂРёР°Р» materialIndex
		const int materialSubMeshIndex =
			mesh.GetMaterialSubMesh(materialIndex);

		// Р•СЃР»Рё РІ РґР°РЅРЅРѕР№ СЃРµС‚РєРµ РЅРµС‚ РіСЂР°РЅРµР№, РёСЃРїРѕР»СЊР·СѓСЋС‰РёС… РјР°С‚РµСЂРёР°Р»
		// materialIndex, С‚Рѕ РІС‹С…РѕРґРёРј, С‚.Рє. СЂРёСЃРѕРІР°С‚СЊ РЅРµС‡РµРіРѕ
		if (materialSubMeshIndex < 0)
		{
			// С‚.Рє. РјС‹ РЅРёС‡РµРіРѕ РЅРµ СЂРёСЃСѓРµРј, С‚Рѕ СЃРѕРѕР±С‰Р°РµРј, С‡С‚Рѕ РјР°С‚РµСЂРёР°Р»
			// РјС‹ РЅРµ Р°РєС‚РёРІРёСЂРѕРІР°Р»Рё
			return false;
		}

		bool materialActivated = false;

		if (activateMaterial && (materialIndex >= 0))
		{
			ModelMaterial const& modelMaterial = model.GetMaterial(materialIndex);

			modelMaterial.GetMaterial().Activate(GL_FRONT_AND_BACK);

			if (modelMaterial.HasTextureMap1())
			{
				glEnable(GL_TEXTURE_2D);
				modelMaterial.GetTextureMap1().GetTexture().Bind();
				m_texture2DEnabled = true;
			}
			else if (m_texture2DEnabled)
			{
				glDisable(GL_TEXTURE_2D);
				m_texture2DEnabled = false;
			}

			materialActivated = true;

			if (modelMaterial.IsTwoSided())
			{
				if (m_cullFace)
				{
					glDisable(GL_CULL_FACE);
					m_cullFace = false;
					glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
				}
			}
			else
			{
				if (!m_cullFace)
				{
					glEnable(GL_CULL_FACE);
					m_cullFace = true;
					glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
				}
			}
		}

		GLubyte const* pVertexPointer = reinterpret_cast<GLubyte const*>
			(model.GetVertexBuffer().GetBufferPointer());

		SetupMeshVertexPointers(mesh, pVertexPointer);

		bool needToUnlockArrays = false;
		if (
			(mesh.GetIndexCount() > mesh.GetVertexCount() * 2) &&
			GLEW_EXT_compiled_vertex_array
			)
		{
			glLockArraysEXT(0, mesh.GetVertexCount());
			needToUnlockArrays = true;
		}

		GLubyte const* pIndexPointer = reinterpret_cast<GLubyte const*>
			(model.GetIndexBuffer().GetBufferPointer());

		RenderSubMeshFaces(
			mesh,
			materialSubMeshIndex,
			pIndexPointer + mesh.GetIndexBufferOffset());

		if (needToUnlockArrays)
		{
			glUnlockArraysEXT();
		}

		return materialActivated;
	}

	void SetupMeshVertexPointers(
		Mesh const& mesh,					
		GLubyte const* pVertexBufferData	
	)const
	{
		unsigned vertexBufferOffset = mesh.GetVertexBufferOffset();

		// Р•СЃС‚СЊ Р»Рё РІ СЃРµС‚РєРµ С‚РµРєСЃС‚СѓСЂРЅС‹Рµ РєРѕРѕСЂРґРёРЅР°С‚С‹?
		bool meshUsesTexture = mesh.HasTextureCoords();

		// Р’С‹С‡РёСЃР»СЏРµРј РёРЅС‚РµСЂРІР°Р» РјРµР¶РґСѓ РІРµСЂС€РёРЅР°РјРё РїРѕР»РёРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		// РІ Р·Р°РІРёСЃРёРјРѕСЃС‚Рё РѕС‚ РЅР°Р»РёС‡РёСЏ С‚РµРєСЃС‚СѓСЂРЅС‹С… РєРѕРѕСЂРґРёРЅР°С‚
		unsigned stride =
			meshUsesTexture ? sizeof(TexturedVertex) : sizeof(Vertex);

		// Р—Р°РґР°РµРј Р°РґСЂРµСЃР° РЅР°С‡Р°Р»Р° РјР°СЃСЃРёРІРѕРІ РІРµСЂС€РёРЅ Рё РЅРѕСЂРјР°Р»РµР№ С‚РµРєСѓС‰РµР№
		// РїРѕР»РёРѕРіРѕРЅР°Р»СЊРЅРѕР№ СЃРµС‚РєРё
		glVertexPointer(
			3,
			GL_FLOAT,
			stride,
			pVertexBufferData + vertexBufferOffset + offsetof(Vertex, position)
		);
		glNormalPointer(
			GL_FLOAT,
			stride,
			pVertexBufferData + vertexBufferOffset + offsetof(Vertex, normal)
		);

		if (meshUsesTexture && !m_texCoordsEnabled)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		else if (!meshUsesTexture && m_texCoordsEnabled)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		// Р•СЃР»Рё СЃРµС‚РєР° РёСЃРїРѕР»СЊР·СѓРµС‚ С‚РµРєСЃС‚СѓСЂРЅС‹Рµ РєРѕРѕСЂРґРёРЅР°С‚С‹, Р·Р°РґР°РµРј
		// Р°РґСЂРµСЃ РЅР°С‡Р°Р»Р° РјР°СЃСЃРёРІР° С‚РµРєСЃС‚СѓСЂРЅС‹С… РєРѕРѕСЂРґРёРЅР°С‚
		if (meshUsesTexture)
		{
			glTexCoordPointer(
				2,
				GL_FLOAT,
				stride,
				pVertexBufferData +
				vertexBufferOffset +
				offsetof(TexturedVertex, texCoord));
		}

		// РћР±РЅРѕРІР»СЏРµРј РёРЅС„РѕСЂРјР°С†РёСЋ Рѕ С‚РѕРј, Р±С‹Р» Р»Рё РІРєР»СЋС‡РµРЅ РјР°СЃСЃРёРІ С‚РµРєСЃС‚СѓСЂРЅС‹С… РєРѕРѕСЂРґРёРЅР°С‚
		m_texCoordsEnabled = meshUsesTexture;
	}

	// Р’РёР·СѓР°Р»РёР·Р°С†РёСЏ РіСЂР°РЅРµР№ РїРѕРґСЃРµС‚РєРё
	// РџСЂРµРґРїРѕР»Р°РіР°РµС‚СЃСЏ, С‡С‚Рѕ Р°РґСЂРµСЃР° РјР°СЃСЃРёРІРѕРІ РІРµСЂС€РёРЅ, РЅРѕСЂРјР°Р»РµР№ Рё С‚РµРєСЃС‚СѓСЂРЅС‹С…
	// РєРѕРѕСЂРґРёРЅР°С‚ СѓР¶Рµ РЅР°СЃС‚СЂРѕРµРЅС‹ РЅР° С‚РµРєСѓС‰СѓСЋ СЃРµС‚РєРё
	void RenderSubMeshFaces(
		Mesh const& mesh,				// СЃРµС‚РєР°
		unsigned subMeshIndex,			// РёРЅРґРµРєСЃ СЂРёСЃСѓРµРјРѕР№ РїРѕРґСЃРµС‚РєРё
		GLubyte const* pMeshIndices	// Р°РґСЂРµСЃ РјР°СЃСЃРёРІР° РёРЅРґРµРєСЃРѕРІ СЃРµС‚РєРё
	)const
	{
		// РїРѕР»СѓС‡Р°РµРј РїРѕРґСЃРµС‚РєСѓ СЃ РёРЅРґРµРєСЃРѕРј subMeshIndex
		Mesh::SubMesh const subMesh = mesh.GetSubMesh(subMeshIndex);

		// Р’С‹С‡РёСЃР»СЏРµРј Р°РґСЂРµСЃ РїРѕРґСЃРµС‚РєРё РІ РёРЅРґРµРєСЃРЅРѕРј Р±СѓС„РµСЂРµ
		GLubyte const* pSubMeshPointer =
			pMeshIndices + (subMesh.startIndex * mesh.GetIndexSize());

		// Р•СЃР»Рё РїРѕРґРґРµСЂР¶РёРІР°РµС‚СЃСЏ СЂР°СЃС€РёСЂРµРЅРёРµ GL_EXT_draw_range_elements,
		// РёСЃРїРѕР»СЊР·СѓРµРј РµРіРѕ РґР»СЏ СЂРёСЃРѕРІР°РЅРёСЏ РјР°СЃСЃРёРІР° РїСЂРёРјРёС‚РёРІРѕРІ,
		// С‚.Рє. РµРіРѕ СЂРµР°Р»РёР·Р°С†РёСЏ РјРѕР¶РµС‚ Р±С‹С‚СЊ Р±РѕР»РµРµ СЌС„С„РµРєС‚РёРІРЅРѕР№
		// РїРѕ СЃСЂР°РІРЅРµРЅРёСЋ c glDrawElements
		if (GLEW_EXT_draw_range_elements)
		{
			// РСЃРїРѕР»СЊР·СѓРµРј Р±РѕР»РµРµ РїСЂРѕРёР·РІРѕРґРёС‚РµР»СЊРЅС‹Р№ СЃРїРѕСЃРѕР± СЂРёСЃРѕРІР°РЅРёСЏ 
			glDrawRangeElements(
				mesh.GetPrimitiveType(),
				0,
				mesh.GetVertexCount() - 1,
				subMesh.indexCount,
				mesh.GetIndexType(),
				pSubMeshPointer
			);
		}
		else
		{
			// Р•СЃР»Рё СЂР°СЃС€РёСЂРµРЅРёРµ GL_EXT_draw_range_elements РЅРµ РїРѕРґРґРµСЂР¶РёРІР°РµС‚СЃСЏ,
			// СЂРёСЃСѓРµРј С‚СЂР°РґРёС†РёРѕРЅРЅС‹Рј СЃРїРѕСЃРѕР±РѕРј
			glDrawElements(
				mesh.GetPrimitiveType(),
				subMesh.indexCount,
				mesh.GetIndexType(),
				pSubMeshPointer
			);
		}
	}

	// Р‘С‹Р» Р»Рё РІРєР»СЋС‡РµРЅ РјР°СЃСЃРёРІ С‚РµРєСЃС‚СѓСЂРЅС‹С… РєРѕРѕСЂРґРёРЅР°С‚?
	mutable bool m_texCoordsEnabled;
	// Tracks whether GL_TEXTURE_2D is enabled for the current material.
	mutable bool m_texture2DEnabled;
	// Р‘С‹Р» Р»Рё РІРєР»СЋС‡РµРЅ СЂРµР¶РёРј РѕС‚Р±СЂР°РєРѕРІРєРё РіСЂР°РЅРµР№?
	mutable bool m_cullFace;

};
