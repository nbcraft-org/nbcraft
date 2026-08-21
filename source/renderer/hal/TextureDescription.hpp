#pragma once

#include <stdint.h>
#include "ImageDescription.hpp"
#include "enums/TextureFiltering.hpp"

namespace mce
{
	struct TextureDescription : public ImageDescription
	{
        unsigned int sampleCount;
        unsigned int mipCount;
        bool bWrap;
        TextureFiltering filteringLevel;
        bool bIsStaging;
		
		TextureDescription()
		{
			sampleCount = 1;
			mipCount = 1;
			bWrap = false;
			filteringLevel = TEXTURE_FILTERING_POINT;
			bIsStaging = false;
		}
	};
}
