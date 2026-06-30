#pragma once

#include <string>
#include "IntRectangle.hpp"

struct SpriteDef
{
	std::string sprite;
	IntRectangle rect;

	SpriteDef()
	{
	}

	SpriteDef(const std::string& sprite, const IntRectangle& rect)
		: sprite(sprite)
		, rect(rect)
	{
	}

	bool isValid() const
	{
		return !sprite.empty();
	}
};
