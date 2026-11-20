/********************************************************************
	NBCraft

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <string>
#include "common/Utils.hpp"

// Indev-style world types (finite map)
enum IndevWorldType
{
	INDEV_TYPE_ISLAND,
	INDEV_TYPE_FLOATING,
	INDEV_TYPE_FLAT,
	INDEV_TYPE_INLAND,
	INDEV_TYPE_COUNT
};

// Indev-style world themes
enum IndevWorldTheme
{
	INDEV_THEME_NORMAL,
	INDEV_THEME_HELL,
	INDEV_THEME_PARADISE,
	INDEV_THEME_WOODS,
	INDEV_THEME_COUNT
};

struct IndevThemeColors
{
	uint32_t sky;
	uint32_t fog;
	uint32_t cloud;
	int skyBrightness; // 0-15, the sky light level of the theme
};

namespace Indev
{
	inline const char* GetTypeName(IndevWorldType type)
	{
		switch (type)
		{
		case INDEV_TYPE_ISLAND:   return "Island";
		case INDEV_TYPE_FLOATING: return "Floating";
		case INDEV_TYPE_FLAT:     return "Flat";
		case INDEV_TYPE_INLAND:   return "Inland";
		default:                  return "Unknown";
		}
	}

	inline const char* GetThemeName(IndevWorldTheme theme)
	{
		switch (theme)
		{
		case INDEV_THEME_NORMAL:   return "Normal";
		case INDEV_THEME_HELL:     return "Hell";
		case INDEV_THEME_PARADISE: return "Paradise";
		case INDEV_THEME_WOODS:    return "Woods";
		default:                   return "Unknown";
		}
	}

	inline IndevThemeColors GetThemeColors(IndevWorldTheme theme)
	{
		IndevThemeColors c;
		switch (theme)
		{
		case INDEV_THEME_HELL:
			c.sky = 0x210800; c.fog = 0x100400; c.cloud = 0x210800; c.skyBrightness = 7;
			break;
		case INDEV_THEME_PARADISE:
			c.sky = 0xC6DEFF; c.fog = 0xEEEEFF; c.cloud = 0xEEEEFF; c.skyBrightness = 15;
			break;
		case INDEV_THEME_WOODS:
			c.sky = 0x757D87; c.fog = 0x4D5A5B; c.cloud = 0x4D5A5B; c.skyBrightness = 12;
			break;
		case INDEV_THEME_NORMAL:
		default:
			c.sky = 0x99CCFF; c.fog = 0xFFFFFF; c.cloud = 0xFFFFFF; c.skyBrightness = 15;
			break;
		}
		return c;
	}

	// Content that existed in Indev. Anything else is hidden from the
	// creative inventory and has its crafting recipes removed.
	bool IsTileAvailable(int tileId);
	bool IsItemAvailable(int itemId);
}
