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

Color Color::getHSBColor(float h, float s, float b)
{
	return Color(Mth::HSBtoRGB(h, s, b));
}