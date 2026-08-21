#pragma once

#include "enums/ComparisonFunc.hpp"

namespace mce
{
	struct RenderStateDescription
	{
		bool enableTexture;
		bool rescaleNormals;

		RenderStateDescription()
		{
			enableTexture = false;
			rescaleNormals = false;
		}

		bool operator==(const RenderStateDescription& other) const
		{
			return enableTexture  == other.enableTexture
				&& rescaleNormals == other.rescaleNormals;
		}

		bool operator!=(const RenderStateDescription& other) const
		{
			return enableTexture  != other.enableTexture
				|| rescaleNormals != other.rescaleNormals;
		}
	};
}
