#pragma once

#include "common/math/Color.hpp"
#include "enums/FogMode.hpp"

namespace mce
{
	struct FogStateDescription
	{
		bool enableFog;
		FogMode fogMode;
		float fogDensity;
		float fogStartZ;
		float fogEndZ;
		Color fogColor;

		FogStateDescription()
		{
			enableFog = false;
			fogDensity = 1.0f;
			fogStartZ = 0.0f;
			fogEndZ = 0.0f;
			fogMode = FOG_MODE_EXP;
		}

		bool operator==(const FogStateDescription& other) const
		{
			return enableFog  == other.enableFog
				&& fogMode    == other.fogMode
				&& fogDensity == other.fogDensity
				&& fogStartZ  == other.fogStartZ
				&& fogEndZ    == other.fogEndZ
				&& fogColor   == other.fogColor;
		}

		bool operator!=(const FogStateDescription& other) const
		{
			return enableFog  != other.enableFog
				|| fogMode    != other.fogMode
				|| fogDensity != other.fogDensity
				|| fogStartZ  != other.fogStartZ
				|| fogEndZ    != other.fogEndZ
				|| fogColor   != other.fogColor;
		}
	};
}
