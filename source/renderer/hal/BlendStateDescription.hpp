#pragma once

#include "enums/BlendTarget.hpp"
#include "enums/ColorWriteMask.hpp"

namespace mce
{
	struct BlendStateDescription
	{
		BlendTarget blendSource;
		BlendTarget blendDestination;
		ColorWriteMask colorWriteMask;
		bool enableBlend;
		
		BlendStateDescription()
		{
			blendSource = BLEND_TARGET_SOURCE_ALPHA;
			blendDestination = BLEND_TARGET_ONE_MINUS_SRC_ALPHA;
			colorWriteMask = COLOR_WRITE_MASK_ALL;
			enableBlend = false;
		}
		
		bool operator==(const BlendStateDescription& other) const
		{
			return blendSource      == other.blendSource
				&& blendDestination == other.blendDestination
				&& colorWriteMask   == other.colorWriteMask
				&& enableBlend      == other.enableBlend;
		}

		bool operator!=(const BlendStateDescription& other) const
		{
			return blendSource      != other.blendSource
				|| blendDestination != other.blendDestination
				|| colorWriteMask   != other.colorWriteMask
				|| enableBlend      != other.enableBlend;
		}
	};
}
