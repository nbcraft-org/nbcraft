#include <math.h>

#include "Color.hpp"
#include "common/Mth.hpp"

Color Color::SHADE_WEST_EAST   = Color(0.6f, 0.6f, 0.6f);
Color Color::SHADE_NORTH_SOUTH = Color(0.8f, 0.8f, 0.8f);
Color Color::SHADE_UP          = Color(1.0f, 1.0f, 1.0f);
Color Color::SHADE_DOWN        = Color(0.5f, 0.5f, 0.5f);

Color Color::NIL    = Color(0.0f, 0.0f, 0.0f, 0.0f);
Color Color::CYAN   = Color(0.0f, 1.0f, 1.0f);
Color Color::PURPLE = Color(1.0f, 0.0f, 1.0f);
Color Color::YELLOW = Color(1.0f, 1.0f, 0.0f);
Color Color::BLUE   = Color(0.0f, 0.0f, 1.0f);
Color Color::GREEN  = Color(0.0f, 1.0f, 0.0f);
Color Color::RED    = Color(1.0f, 0.0f, 0.0f);
Color Color::BLACK  = Color(0.0f, 0.0f, 0.0f);
Color Color::GREY   = Color(0.5f, 0.5f, 0.5f);
Color Color::WHITE  = Color(1.0f, 1.0f, 1.0f);

Color Color::TEXT_GREY = Color(0x404040);

Color Color::getHSBColor(float hue, float saturation, float brightness)
{
	Color c = Color::WHITE;
	if (saturation == 0)
		c.r = c.g = c.b = brightness;
	else
	{
		float h = (hue - Mth::floor(hue)) * 6.0f;
		float f = h - Mth::floor(h);
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));
		switch ((int)h)
		{
		case 0:
			c.r = brightness;
			c.g = t;
			c.b = p;
			break;
		case 1:
			c.r = q;
			c.g = brightness;
			c.b = p;
			break;
		case 2:
			c.r = p;
			c.g = brightness;
			c.b = t;
			break;
		case 3:
			c.r = p;
			c.g = q;
			c.b = brightness;
			break;
		case 4:
			c.r = t;
			c.g = p;
			c.b = brightness;
			break;
		case 5:
			c.r = brightness;
			c.g = p;
			c.b = q;
			break;
		}
	}
	return c;
}
