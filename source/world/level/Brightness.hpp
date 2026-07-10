#pragma once
#include <stdint.h>

// int8 to prevent overflow issues
typedef int8_t Brightness_t;

class Brightness
{
public:
	static const Brightness_t MIN;
	static const Brightness_t MAX;
};
