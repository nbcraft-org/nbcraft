#include "GrassColor.hpp"
#include "compat/EndianDefinitions.h"
#include "client/resources/Resource.hpp"

bool GrassColor::_isAvailable = false;

TextureData GrassColor::texture;

void GrassColor::init()
{
	GrassColor::texture = Resource::loadTexture("misc/grasscolor.png");
}

uint32_t GrassColor::get(float x, float y)
{
	y *= x;
	uint32_t c = GrassColor::texture.getData()[(int)((1.0f - y) * 255.0f) << 8 | (int)((1.0f - x) * 255.0f)];
	
#if MC_ENDIANNESS_BIG
	uint8_t r = c & 0xFF, g = (c >> 8) & 0xFF, b = (c >> 16) & 0xFF, a = (c >> 24) & 0xFF;
	c = a | (b << 8) | (g << 16) | (r << 24);
#endif
	
	return c;
}
