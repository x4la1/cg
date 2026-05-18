#pragma once
#include "pch.h"
#include <sstream>
#include "ModelLoader.h"

std::string RgbToString(const float rgb[3])
{
	std::stringstream s;
	s << "(" << rgb[0] << "; " << rgb[1] << "; " << rgb[2] << ")";
	return s.str();
}

void DumpMaterial(const Lib3dsFile* pFile)
{
	const int materialsCount = pFile->nmaterials;
	std::cout << "===Materials===\n";
	for (int i = 0; i < materialsCount; ++i)
	{
		Lib3dsMaterial const* pMaterial = pFile->materials[i];

		//Material properties
		std::cout << "\t" << "Material: '" << pMaterial->name << "'\n";
		std::cout << "\t\t" << "Ambient: " << RgbToString(pMaterial->ambient) << "\n";
		std::cout << "\t\t" << "Diffuse: " << RgbToString(pMaterial->diffuse) << "\n";
		std::cout << "\t\t" << "Specular: " << RgbToString(pMaterial->specular) << "\n";
		std::cout << "\t\t" << "Shininess: " << pMaterial->shininess << "\n";

		Lib3dsTextureMap const& textureMap = pMaterial->texture1_map;

		if (textureMap.name[0] != '\0')
		{
			std::cout << "\t\t" << "Texture: " << textureMap.name << "\n";
		}
	}
}

std::string VectorToString(float const vec[3])
{
	std::stringstream s;
	s << "(" << vec[0] << "; " << vec[1] << "; " << vec[2] << ")";
	return s.str();
}

void DumpLights(const Lib3dsFile* pFile)
{
	std::cout << "===Lights===\n";
	const int lightsCount = pFile->nlights;
	for (int i = 0; i < lightsCount; ++i)
	{
		Lib3dsLight const* pLight = pFile->lights[i];
		std::cout << "\t" << "Light: '" << pLight->name << "'\n";
		std::cout << "\t\t" << "Color: " << RgbToString(pLight->color) << "\n";
		std::cout << "\t\t" << "Position: " << VectorToString(pLight->position) << "\n";
	}
}

std::string TextureCoordsToString(float const vec[2])
{
	std::stringstream s;
	s << "(" << vec[0] << "; " << vec[1] << ")";
	return s.str();
}

void DumpMeshes(const Lib3dsFile* pFile)
{
	std::cout << "===Meshes==\n";
	const int meshCount = pFile->nmeshes;
	for (int i = 0; i < meshCount; ++i)
	{
		Lib3dsMesh const* pMesh = pFile->meshes[i];

		std::cout << "\t" << "Mesh: '" << pMesh->name << "'\n";

		// Vertices
		{
			const int vertexCount = pMesh->nvertices;
			std::cout << "\t\t" << "Vertices: " << vertexCount << "\n";
			float const(*vertices)[3] = pMesh->vertices;
			float const(*textureCoords)[2] = pMesh->texcos;

			for (int i = 0; i < vertexCount; ++i)
			{
				std::cout << "\t\t\tVertex[" << i << "] " << VectorToString(vertices[i]);
				if (textureCoords != NULL)
				{
					// Tex coords
					std::cout << " Tex: " << TextureCoordsToString(textureCoords[i]);
				}
				std::cout << "\n";
			}
		}

		// Faces
		{
			const int faceCount = pMesh->nfaces;
			std::cout << "\t\t" << "Faces: " << faceCount << "\n";
			for (int f = 0; f < faceCount; ++f)
			{
				Lib3dsFace const& face = pMesh->faces[f];
				const unsigned short* indices = face.index;
				std::cout << "\t\t\tFace[" << f << "] "
					<< "(" << indices[0] << ", " << indices[1] << ", "
					<< indices[2] << ")"
					<< " smooth: " << face.smoothing_group << " flags:" << face.flags
					<< " material: '" << face.material << "'\n";
			}
		}
	}
}

void Dump3dsFile(const char* fileName)
{
	Lib3dsFile* pFile = lib3ds_file_open(fileName);
	if (pFile == NULL)
	{
		throw std::runtime_error("Unable to read 3ds file");
	}

	DumpMaterial(pFile);
	//DumpLights(pFile);
	//DumpMeshes(pFile);
	lib3ds_file_free(pFile);
}