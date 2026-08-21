#pragma once

#include "enums/CullMode.hpp"

namespace mce
{
	struct RasterizerStateDescription
	{
        float depthBias;
        CullMode cullMode;
        bool enableScissorTest;
	
		RasterizerStateDescription()
		{
			depthBias = 0.0f;
			cullMode = CULL_BACK;
			enableScissorTest = false;
		}

		bool operator==(const RasterizerStateDescription& other) const
		{
			return depthBias == other.depthBias
				&& cullMode  == other.cullMode;
		}

		bool operator!=(const RasterizerStateDescription& other) const
		{
			return depthBias != other.depthBias
				|| cullMode  != other.cullMode;
		}
	};
}
