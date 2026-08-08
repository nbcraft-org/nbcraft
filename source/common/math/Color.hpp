#pragma once

#include <stdint.h>

#define GET_RED(c)   (uint8_t(((c) >>  0) & 0xFF))
#define GET_GREEN(c) (uint8_t(((c) >>  8) & 0xFF))
#define GET_BLUE(c)  (uint8_t(((c) >> 16) & 0xFF))
#define GET_ALPHA(c) (uint8_t(((c) >> 24) & 0xFF))

struct Color
{
public:
    float r;
    float g;
    float b;
    float a;

private:
    void _init(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    void _init(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        this->r = float(r) / 255.0f;
        this->g = float(g) / 255.0f;
        this->b = float(b) / 255.0f;
        this->a = float(a) / 255.0f;
    }

    void _init(uint8_t r, uint8_t g, uint8_t b, float a)
    {
        _init(r, g, b, (uint8_t)255);
        this->a = a;
    }

public:
    Color()
    {
		*this = NIL;
    }

    Color(float r, float g, float b, float a = 1.0f)
    {
        _init(r, g, b, a);
    }

    Color(int r, int g, int b, int a = 255)
    {
        _init((uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a);
    }

    Color(int r, int g, int b, float a)
    {
        _init((uint8_t)r, (uint8_t)g, (uint8_t)b, a);
    }

    Color(unsigned int c)
    {
        _init(GET_RED(c), GET_GREEN(c), GET_BLUE(c), GET_ALPHA(c));
    }

    Color(unsigned int c, float alpha)
    {
        _init(GET_RED(c), GET_GREEN(c), GET_BLUE(c), alpha);
    }

    static Color getHSBColor(float h, float s, float b);

    Color& mulRGB(float mul)
    {
        r *= mul;
        g *= mul;
        b *= mul;
        return *this;
    }

    Color& withAlpha(int alpha)
    {
        return withAlpha(alpha / 255.0f);
    }

    Color& withAlpha(float alpha)
    {
        a = alpha;
        return *this;
    }

    Color operator*(float f) const
    {
        return Color(r * f, g * f, b * f, a);
    }

    Color operator*(const Color& c) const
    {
        return Color(r * c.r, g * c.g, b * c.b, a * c.a);
    }

    Color& operator*=(const Color& c)
    {
        r *= c.r;
        g *= c.g;
        b *= c.b;
        a *= c.a;

        return *this;
    }

    Color& operator*=(float f)
    {
        r *= f;
        g *= f;
        b *= f;

        return *this;
    }

    operator int() const
    {
        // Xbox 360 for some reason expects the colors as little-endian
#if MC_ENDIANNESS_BIG && !defined(_XBOX)
        return (int(a * 255) | (int(r * 255) << 8) | (int(g * 255) << 16) | (int(b * 255) << 24));
#else // MC_ENDIANNESS_LITTLE
        return ((int(a * 255) << 24) | (int(r * 255) << 16) | (int(g * 255) << 8) | int(b * 255));
#endif
    }

    bool operator==(const Color& other) const
    {
        return this->r == other.r
            && this->g == other.g
            && this->b == other.b
            && this->a == other.a;
    }

    bool operator!=(const Color& other) const
    {
        return this->r != other.r
            || this->g != other.g
            || this->b != other.b
            || this->a != other.a;
    }

public:
    static Color SHADE_WEST_EAST;
    static Color SHADE_NORTH_SOUTH;
    static Color SHADE_UP;
    static Color SHADE_DOWN;

    static Color NIL;
    static Color CYAN;
    static Color PURPLE;
    static Color YELLOW;
    static Color BLUE;
    static Color GREEN;
    static Color RED;
    static Color BLACK;
    static Color GREY;
    static Color WHITE;

    static Color TEXT_GREY;
};
