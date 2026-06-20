#pragma once

#include "Button.hpp"

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

class TabButton : public Button
{
public:
	TabButton(int width, int height, const std::string& text, const SpriteDef& highImage, const SpriteDef& downImage, const SpriteDef& iconImage);

protected:
	virtual void _renderImage(Minecraft&, const SpriteDef&);
	virtual void _renderMessage(Minecraft&);

public:
	void renderBg(Minecraft*, const MenuPointer& pointer) override;
	void render(Minecraft* pMinecraft, const MenuPointer& pointer) override;

public:
	SpriteDef m_highImage;
	SpriteDef m_downImage;
	SpriteDef m_iconImage;
};

