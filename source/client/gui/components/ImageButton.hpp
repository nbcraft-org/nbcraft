#pragma once

#include "Button.hpp"

struct ImageDef
{
	std::string name;
	int xOffset;
	int yOffset;
	float width;
	float height;
	IntRectangle uv;
	bool bUseCustomUV;

	ImageDef()
		: xOffset(0)   , yOffset(0)
		, width(16.0f) , height(16.0f)
		, bUseCustomUV(false)
	{
	}
};

class ImageButton : public Button
{
private:
	void _init();

public:
	ImageButton(const std::string& text);
	ImageButton(const std::string& text, const ImageDef& imageDef);

protected:
	virtual void _renderImage(Minecraft&, bool isPressed);
	virtual void _renderMessage(Minecraft&, bool isPressed);

public:
	void renderBg(Minecraft*, const MenuPointer& pointer) override;
	void render(Minecraft* pMinecraft, const MenuPointer& pointer) override;

public:
	void setImageDef(const ImageDef& imageDef, bool setSize = true);

public:
	virtual void setupDefault();
	virtual bool isSecondImage(bool isPressed) const { return isPressed; }

public:
	ImageDef m_image;
	bool m_bScaleOnPress;
};

