#pragma once

#include "Button.hpp"

class THeader_Pocket : public Button
{
private:
	void _init();

public:
	THeader_Pocket(int x, int y, int width, int height, const std::string& text);
	THeader_Pocket(int x, int y, const std::string& text);
	THeader_Pocket(const std::string& text);

public:
	void renderBg(Minecraft*, const MenuPointer& pointer) override;
	void render(Minecraft* pMinecraft, const MenuPointer& pointer) override;

public:
	int m_textPosX;
};

