#include "FoliageColor.hpp"
#include "compat/EndianDefinitions.h"
#include "client/resources/Resource.hpp"

bool FoliageColor::_isAvailable = false;

TextureData FoliageColor::texture;

void FoliageColor::init()
{
	FoliageColor::texture = Resource::loadTexture("misc/foliagecolor.png");
}

uint32_t FoliageColor::get(float x, float y)
{
	y *= x;
	uint32_t c = FoliageColor::texture.getData()[(int)((1.0f - y) * 255.0f) << 8 | (int)((1.0f - x) * 255.0f)];
	
	// @TODO: same as in GrassColor::get, should be abstracted
#if MC_ENDIANNESS_BIG
	uint8_t r = c & 0xFF, g = (c >> 8) & 0xFF, b = (c >> 16) & 0xFF, a = (c >> 24) & 0xFF;
	c = a | (b << 8) | (g << 16) | (r << 24);
#endif
	
	return c;
}

uint32_t FoliageColor::getEvergreenColor()
{
	return 0xFF619961;
}

uint32_t FoliageColor::getBirchColor()
{
	return 0xFF55A780;
}

uint32_t FoliageColor::getDefaultColor()
{
	return 0xFF18B548;
}
