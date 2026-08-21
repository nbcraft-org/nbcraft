#pragma once

#include "renderer/hal/enums/VertexField.hpp"

namespace mce
{
	class Attribute
	{
	private:
		VertexField m_vertexField;
		unsigned int m_location;
		unsigned int m_count;
		
	public:
		Attribute()
		{
			m_vertexField = VertexField();
			m_location = 0;
			m_count = 0;
		}

		Attribute(unsigned int location, unsigned int count, VertexField vertexField)
		{
			m_vertexField = vertexField;
			m_location = location;
			m_count = count;
		}

		VertexField getVertexField() const { return m_vertexField; }
		unsigned int getLocation() const { return m_location; }
		
		bool operator==(const Attribute& other) const
		{
		  return m_vertexField == other.m_vertexField
			  && m_location    == other.m_location
			  && m_count       == other.m_count;
		}

		bool operator!=(const Attribute& other) const
		{
		  return m_vertexField != other.m_vertexField
			  || m_location    != other.m_location
			  || m_count       != other.m_count;
		}
	};
}
