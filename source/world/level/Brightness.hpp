#pragma once
#include <stdint.h>

typedef uint8_t Brightness_t;

class Brightness
{
public:
	static const Brightness_t MIN;
	static const Brightness_t MAX;
	static const Brightness_t DECAY;
};
