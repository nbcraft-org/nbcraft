#pragma once

#include "enums/ComparisonFunc.hpp"
#include "enums/StencilOp.hpp"

namespace mce
{
	struct StencilFaceDescription
	{
		ComparisonFunc stencilFunc;
		StencilOp stencilDepthFailOp;
		StencilOp stencilPassOp;
		StencilOp stencilFailOp;
		
		StencilFaceDescription()
		{
			stencilFunc = COMPARISON_FUNC_ALWAYS;
			stencilDepthFailOp = STENCIL_OP_KEEP;
			stencilPassOp = STENCIL_OP_KEEP;
			stencilFailOp = STENCIL_OP_KEEP;
		}

		bool operator==(const StencilFaceDescription& other) const
		{
			return stencilFunc        == other.stencilFunc
				&& stencilDepthFailOp == other.stencilDepthFailOp
				&& stencilPassOp      == other.stencilPassOp
				&& stencilFailOp      == other.stencilFailOp;
		}

		bool operator!=(const StencilFaceDescription& other) const
		{
			return stencilFunc        != other.stencilFunc
				|| stencilDepthFailOp != other.stencilDepthFailOp
				|| stencilPassOp      != other.stencilPassOp
				|| stencilFailOp      != other.stencilFailOp;
		}
	};
}
