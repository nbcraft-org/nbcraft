#pragma once

#include "ImageButton.hpp"

class LargeImageButton : public ImageButton
{
private:
	void _init();

public:
	LargeImageButton(const std::string& text);
	LargeImageButton(const std::string& text, const ImageDef& imageDef);

protected:
	void _renderImage(Minecraft&, bool isPressed) override;
	void _renderMessage(Minecraft&, bool isPressed) override;

public:
	void setupDefault() override;

public:
	float m_unknown;
};

