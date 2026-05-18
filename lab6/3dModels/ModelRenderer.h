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
		Model const& model,
		unsigned meshIndex,
		int materialIndex,
		bool activateMaterial
	)const
	{
		Mesh const& mesh = model.GetMesh(meshIndex);

		const int materialSubMeshIndex =
			mesh.GetMaterialSubMesh(materialIndex);

		if (materialSubMeshIndex < 0)
		{
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

		bool meshUsesTexture = mesh.HasTextureCoords();

		unsigned stride =
			meshUsesTexture ? sizeof(TexturedVertex) : sizeof(Vertex);

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

		m_texCoordsEnabled = meshUsesTexture;
	}

	void RenderSubMeshFaces(
		Mesh const& mesh,				
		unsigned subMeshIndex,			
		GLubyte const* pMeshIndices
	)const
	{
		Mesh::SubMesh const subMesh = mesh.GetSubMesh(subMeshIndex);

		GLubyte const* pSubMeshPointer =
			pMeshIndices + (subMesh.startIndex * mesh.GetIndexSize());

		if (GLEW_EXT_draw_range_elements)
		{
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
			glDrawElements(
				mesh.GetPrimitiveType(),
				subMesh.indexCount,
				mesh.GetIndexType(),
				pSubMeshPointer
			);
		}
	}

	mutable bool m_texCoordsEnabled;
	mutable bool m_texture2DEnabled;
	mutable bool m_cullFace;
};
