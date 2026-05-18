#pragma once
#include "pch.h"
#include "BoundingBox.h"

class Mesh
{
public:
	// Структура, описывающая подсетку
	struct SubMesh
	{
		// Начало подсетки в массиве индексов сетки
		unsigned startIndex;
		// Количество индексов в подсетке
		unsigned indexCount;
	};

public:
	Mesh(
		unsigned int vertexBufferOffset,	// смещение в буфере вершин
		unsigned int indexBufferOffset,		// смещение в буфере индексов
		unsigned vertexCount,				// количество вершин
		unsigned indexCount,				// количество индексов
		bool hasTextureCoords,				// наличие текстурных координат
		BoundingBox const& boundingBox,	// ограничивающий блок
		GLenum primitiveType,				// тип примитивов сетки
		GLenum indexType					// тип индексов сетки
	) :m_indexBufferOffset(indexBufferOffset)
		, m_vertexBufferOffset(vertexBufferOffset)
		, m_vertexCount(vertexCount)
		, m_indexCount(indexCount)
		, m_hasTexture(hasTextureCoords)
		, m_boundingBox(boundingBox)
		, m_primitiveType(primitiveType)
		, m_indexType(indexType)
	{
	}

	// Есть ли в сетке текстурные координаты?
	bool HasTextureCoords()const
	{
		return m_hasTexture;
	}

	// Возвращаем смещение относительно начала буфера вершин
	unsigned int GetVertexBufferOffset()const
	{
		return m_vertexBufferOffset;
	}

	// Возвращаем смещение относительно начала буфера индексов
	unsigned int GetIndexBufferOffset()const
	{
		return m_indexBufferOffset;
	}

	// Возвращаем количество индексов
	unsigned int GetIndexCount()const
	{
		return m_indexCount;
	}

	// Возвращаем количество вершин
	unsigned int GetVertexCount()const
	{
		return m_vertexCount;
	}

	// Возвращаем ограничивающий блок
	BoundingBox const& GetBoundingBox()const
	{
		return m_boundingBox;
	}

	// Возвращаем тип примитивов (GL_TRIANGLES, GL_TRIANGLE_STRIP и т.п.)
	GLenum GetPrimitiveType()const
	{
		return m_primitiveType;
	}

	// Возвращаем тип данных для хранения индексов (GL_UNSIGNED_SHORT и т.п.)
	GLenum GetIndexType()const
	{
		return m_indexType;
	}

	// Добавляем подсетку, диапазон индексов внутри сетки, объединенных
	// по некоторому признаку (например, по общему материалу)
	// Возвращается индекс добавленной подсетки
	unsigned AddSubMesh(unsigned startIndex, unsigned subMeshIndexCount)
	{
		if (startIndex >= m_indexCount)
		{
			throw std::out_of_range("Start vertex index is out of range");
		}

		// Проверка на допустимость количества индексов в подсетке
		if (
			(subMeshIndexCount == 0) ||
			(startIndex + subMeshIndexCount < startIndex) ||
			(startIndex + subMeshIndexCount > m_indexCount)
			)
		{
			throw std::out_of_range("SubMesh index count is out of range");
		}

		// Создаем подсетку и добавляем в массив подсеток
		SubMesh subMesh = { startIndex, subMeshIndexCount };
		m_subMeshes.push_back(subMesh);

		// Возвращаем индекс вставленной подсетки
		return static_cast<unsigned>(m_subMeshes.size() - 1);
	}

	// Количество подсеток
	unsigned GetSubMeshCount()const
	{
		return static_cast<unsigned>(m_subMeshes.size());
	}

	// Возвращаем информацию о подсетке с заданным индексом
	SubMesh GetSubMesh(unsigned index)const
	{
		return m_subMeshes.at(index);
	}

	// Задаем индекс подсетки, грани которой имеют материал materialIndex
	void SetMaterialSubMesh(int materialIndex, unsigned subMeshIndex)
	{
		if (subMeshIndex >= GetSubMeshCount())
		{
			throw std::out_of_range("Sub mesh index is out of range");
		}

		// Индекс материала, равный -1, задается в подсетках,
		// использующих материал по умолчанию.
		// Т.к. нулевой элемент массива m_materialSubMesh хранит индекс
		// подсетки с материалом равным -1, увеличиваем индекс материала на 1
		++materialIndex;

		// проверяем его на допустимость (-1 - единственное отрицательное
		// значение допустимое для использования в качестве индекса материала)
		if (materialIndex < 0)
		{
			throw std::out_of_range("Invalid material index");
		}

		// Если мы пытаемся задать подсетку для материала, отсутствующего
		// в полигональной сетке, нужно выделить для него место в массиве
		// m_materialSubMesh
		if (static_cast<unsigned>(materialIndex) >= m_materialSubMesh.size())
		{
			m_materialSubMesh.insert(
				m_materialSubMesh.end(),
				materialIndex - m_materialSubMesh.size() + 1,
				-1);
		}

		// Если для данного материала подсетка уже была задана,
		// то не даем переопределить ее
		if (m_materialSubMesh[materialIndex] >= 0)
		{
			throw std::logic_error("Material sub mesh has already specified");
		}

		// добавляем индекс материала в массив используемых материалов,
		// предварительно уменьшив его на 1 (компенсируем инкремент индекса)
		m_materials.push_back(materialIndex - 1);

		// Задаем для данного материала индекс использующей его подсетки
		m_materialSubMesh[materialIndex] = subMeshIndex;
	}

	// Получаем индекс подсетки, грани которой имеют материал materialIndex
	int GetMaterialSubMesh(int materialIndex)const
	{
		// Т.к. индексация элементов массива m_materialSubMesh "виртуально"
	// начинается с -1, увеличиваем индекс материала на 1, не забывая проверить
	// на допустимость значений
		++materialIndex;
		if (materialIndex < 0)
		{
			throw std::out_of_range("Invalid material index");
		}

		// Если индекс материала превышает количество материалов, для которых были
		// указаны индексы подсеток, то возвращаем значение -1, сигнализируя
		// об отсутствии подсетки для данного материала
		if (static_cast<unsigned>(materialIndex) >= m_materialSubMesh.size())
		{
			// нет подсетки, ассоциированной с данным материалом
			return -1;
		}

		// Иначе возвращаем индекс подсетки, соответствующей запрошенному материалу
		return m_materialSubMesh[materialIndex];
	}

	// Получаем количество материалов, используемых в сетке
	unsigned GetMaterialsCount()const
	{
		return static_cast<unsigned>(m_materials.size());
	}

	// Получаем индес материала по его порядковому номеру в сетке
	int GetMaterial(unsigned index)const
	{
		return m_materials.at(index);
	}

	// Получаем количество байт, требуемых для хранения одного индекса
	unsigned GetIndexSize()const
	{
		return
			(m_indexType == GL_UNSIGNED_BYTE) ? sizeof(GLubyte) :
			(m_indexType == GL_UNSIGNED_SHORT) ? sizeof(GLshort) :
			(m_indexType == GL_UNSIGNED_INT) ? sizeof(GLuint) : 0;
	}

private:
	Mesh(const Mesh&) {}
	Mesh& operator=(const Mesh&) {}

	std::vector<SubMesh> m_subMeshes;
	std::vector<int> m_materials;
	std::vector<int> m_materialSubMesh;

	unsigned int m_vertexBufferOffset;
	unsigned int m_indexBufferOffset;
	unsigned int m_vertexCount;
	unsigned int m_indexCount;
	bool m_hasTexture;
	BoundingBox const m_boundingBox;
	GLenum m_primitiveType;
	GLenum m_indexType;
};