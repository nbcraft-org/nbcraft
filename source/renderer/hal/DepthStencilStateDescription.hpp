#pragma once

#include "StencilFaceDescription.hpp"
#include "renderer/StencilRefObject.hpp"
#include "enums/ComparisonFunc.hpp"
#include "enums/DepthWriteMask.hpp"

namespace mce
{
	struct DepthStencilStateDescription
	{
        bool depthTestEnabled;
        bool stencilTestEnabled;
        ComparisonFunc depthFunc;
        StencilFaceDescription frontFace;
        StencilFaceDescription backFace;
        DepthWriteMask depthWriteMask;
        unsigned int stencilReadMask;
        unsigned int stencilWriteMask;
        StencilRefObject stencilRef;
        bool overwroteStencilRef;
		
		DepthStencilStateDescription()
			: stencilRef(0)
		{
			depthFunc = COMPARISON_FUNC_LESS;
			depthTestEnabled = true;
			stencilTestEnabled = false;
			// @TODO: are these two calls redundant?
			frontFace = StencilFaceDescription();
			backFace  = StencilFaceDescription();
			depthWriteMask = DEPTH_WRITE_MASK_ALL;

			overwroteStencilRef = false;
			stencilReadMask  = 0xFFFFFFFF;
			stencilWriteMask = 0xFFFFFFFF;
		}

		bool operator==(const DepthStencilStateDescription& other) const
		{
			return depthFunc           == other.depthFunc
				&& depthTestEnabled    == other.depthTestEnabled
				&& stencilTestEnabled  == other.stencilTestEnabled
				&& frontFace           == other.frontFace
				&& backFace            == other.backFace
				&& depthWriteMask      == other.depthWriteMask
				&& stencilReadMask     == other.stencilReadMask
				&& stencilWriteMask    == other.stencilWriteMask
				&& stencilRef          == other.stencilRef
				&& overwroteStencilRef == other.overwroteStencilRef;
		}

		bool operator!=(const DepthStencilStateDescription& other) const
		{
			return depthFunc           != other.depthFunc
				|| depthTestEnabled    != other.depthTestEnabled
				|| stencilTestEnabled  != other.stencilTestEnabled
				|| frontFace           != other.frontFace
				|| backFace            != other.backFace
				|| depthWriteMask      != other.depthWriteMask
				|| stencilReadMask     != other.stencilReadMask
				|| stencilWriteMask    != other.stencilWriteMask
				|| stencilRef          != other.stencilRef
				|| overwroteStencilRef != other.overwroteStencilRef;
		}
	};
}
