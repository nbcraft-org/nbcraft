#pragma once

#include "renderer/VertexFormat.hpp"

namespace mce
{
	struct VertexBufferStateDescription
	{
		VertexFormat vertexFormat;

		VertexBufferStateDescription()
		{
		}

		VertexBufferStateDescription(const VertexFormat& vertexFormat)
			: vertexFormat(vertexFormat)
		{
		}

		bool operator==(const VertexBufferStateDescription& other) const
		{
			return vertexFormat == other.vertexFormat;
		}

		bool operator!=(const VertexBufferStateDescription& other) const
		{
			return !(*this == other);
		}
	};
}
