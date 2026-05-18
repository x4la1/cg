#pragma once
#include "pch.h"

class BoundingBox
{
public:
	BoundingBox()
		: m_isEmpty(true)
	{
	}

	BoundingBox(glm::dvec3 const& minCoord, glm::dvec3 const& maxCoord)
		: m_minCoord(minCoord)
		, m_maxCoord(maxCoord)
		, m_isEmpty(false)
	{
	}

	bool IsEmpty() const
	{
		return m_isEmpty;
	}

	BoundingBox const Union(BoundingBox const& other) const
	{
		if (m_isEmpty && other.m_isEmpty)
		{
			return BoundingBox();
		}
		else if (m_isEmpty)
		{
			return other;
		}
		else if (other.m_isEmpty)
		{
			return *this;
		}
		else
		{
			using namespace std;
			return BoundingBox(
				glm::dvec3(
					min(m_minCoord.x, other.m_minCoord.x),
					min(m_minCoord.y, other.m_minCoord.y),
					min(m_minCoord.z, other.m_minCoord.z)
				),
				glm::dvec3(
					max(m_maxCoord.x, other.m_maxCoord.x),
					max(m_maxCoord.y, other.m_maxCoord.y),
					max(m_maxCoord.z, other.m_maxCoord.z)
				)
			);
		}
	}

	glm::dvec3 const& GetMinCoord() const
	{
		if (m_isEmpty)
		{
			throw std::logic_error("Bounding box is empty");
		}

		return m_minCoord;
	}

	glm::dvec3 const& GetMaxCoord() const
	{
		if (m_isEmpty)
		{
			throw std::logic_error("Bounding box is empty");
		}

		return m_maxCoord;
	}

	glm::dvec3 const GetSize() const
	{
		if (m_isEmpty)
		{
			return glm::dvec3(0, 0, 0);
		}

		return m_maxCoord - m_minCoord;
	}


	glm::dvec3 const GetCenter() const
	{
		if (m_isEmpty)
		{
			throw std::logic_error("Bounding box is empty");
		}

		return (m_minCoord + m_maxCoord) * 0.5;
	}

private:
	bool m_isEmpty;
	glm::dvec3 m_minCoord;
	glm::dvec3 m_maxCoord;
};