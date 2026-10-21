#include "THeader_Pocket.hpp"
#include "renderer/ShaderConstants.hpp"

void THeader_Pocket::_init()
{
	setEnabled(false);
	m_textPosX = -99999;
}

THeader_Pocket::THeader_Pocket(int x, int y, int width, int height, const std::string& text)
	: Button(x, y, width, height, text)
{
	_init();
}

THeader_Pocket::THeader_Pocket(int x, int y, const std::string& text)
	: Button(x, y, 66, 26, text)
{
	_init();
}

THeader_Pocket::THeader_Pocket(const std::string& text)
	: Button(text)
{
	_init();
	m_width = 66;
	m_height = 26;
}

void THeader_Pocket::renderBg(Minecraft* mc, const MenuPointer& pointer)
{
	Textures& textures = *mc->m_pTextures;
	//blitNineSlice(textures, TextureAtlasSprite(iXPos, 0, 66, 26, "gui/touchgui.png", 256, 256), m_xPos, m_yPos, m_width, m_height, 3, &m_materials.ui_textured_and_glcolor);

	textures.loadAndBindTexture("gui/touchgui.png");
	blit(m_xPos,               m_yPos,                150, 26, 2,           m_height - 1, 2, 25);
	blit(m_xPos + 2,           m_yPos,                153, 26, m_width - 3, m_height - 1, 8, 25);
	blit(m_xPos + m_width - 2, m_yPos,                162, 26, 2,           m_height - 1, 2, 25);

	blit(m_xPos,               m_yPos + m_height - 1, 153, 52, m_width,     3,            8, 3);
}

void THeader_Pocket::render( Minecraft* mc, const MenuPointer& pointer)
{
	currentShaderColor = Color::WHITE;
	renderBg(mc, pointer);

	int textPosX = m_textPosX;
	if (textPosX == -99999)
		textPosX = m_xPos + m_width / 2;

	drawCenteredString(*mc->m_pFont, getMessage(), textPosX, m_yPos + (m_height - 8) / 2, 0xFFE0E0E0);
}
