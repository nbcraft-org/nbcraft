#pragma once

#include "enums/ComparisonFunc.hpp"

namespace mce
{
	struct AlphaStateDescription
	{
		bool enableAlphaTest;
		ComparisonFunc alphaFunc;
		float alphaRef;

		AlphaStateDescription()
		{
			enableAlphaTest = false;
			alphaFunc = COMPARISON_FUNC_GREATER_EQUAL;
			alphaRef = 0.5f;
			// pre-HAL values
			/*alphaFunc = COMPARISON_FUNC_GREATER;
			alphaRef = 0.1f;*/
		}

		bool operator==(const AlphaStateDescription& other) const
		{
			return enableAlphaTest == other.enableAlphaTest
				&& alphaFunc       == other.alphaFunc
				&& alphaRef        == other.alphaRef;
		}

		bool operator!=(const AlphaStateDescription& other) const
		{
			return enableAlphaTest != other.enableAlphaTest
				|| alphaFunc       != other.alphaFunc
				|| alphaRef        != other.alphaRef;
		}
	};
}
