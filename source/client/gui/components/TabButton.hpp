#pragma once

#include "Button.hpp"
#include "client/gui/SpriteDef.hpp"

class TabButton : public Button
{
private:
	void _init();

public:
	TabButton(int width, int height, const std::string& text, const SpriteDef& highImage, const SpriteDef& downImage, const SpriteDef& iconImage, const SpriteDef& downIconImage);
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
	SpriteDef m_downIconImage;
};

