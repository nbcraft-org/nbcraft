#include "TabButton.hpp"
#include "renderer/ShaderConstants.hpp"

void TabButton::_renderImage(Minecraft& mc, const SpriteDef& image)
{
	blitSprite(*mc.m_pTextures, image.sprite, m_xPos + image.rect.x + (m_width - image.rect.w) / 2, m_yPos + image.rect.y + (m_height - image.rect.h) / 2, image.rect.w, image.rect.h);
}

void TabButton::_init()
{
	m_highImage.rect.w = m_downImage.rect.w = m_width;
	m_highImage.rect.h = m_downImage.rect.h = m_height;
}

TabButton::TabButton(int width, int height, const std::string& text, const SpriteDef& highImage, const SpriteDef& downImage, const SpriteDef& iconImage, const SpriteDef& downIconImage)
	: Button(0, 0, width, height, text)
	, m_highImage(highImage)
	, m_downImage(downImage)
	, m_iconImage(iconImage)
	, m_downIconImage(downIconImage)
{
	_init();
}

TabButton::TabButton(int width, int height, const std::string& text, const SpriteDef& highImage, const SpriteDef& downImage, const SpriteDef& iconImage)
	: Button(0, 0, width, height, text)
	, m_highImage(highImage)
	, m_downImage(downImage)
	, m_iconImage(iconImage)
	, m_downIconImage(iconImage)
{
	_init();
}

void TabButton::_renderMessage(Minecraft& mc)
{
}

void TabButton::renderBg(Minecraft* pMinecraft, const MenuPointer& pointer)
{
	_renderImage(*pMinecraft, isSelected() ? m_highImage : m_downImage);
}

void TabButton::render(Minecraft* pMinecraft, const MenuPointer& pointer)
{
	if (!isVisible()) return;

	if (m_color.a == 0.0f)
		return;

	currentShaderColor = m_color;

    renderBg(pMinecraft, pointer);
	
	if (m_iconImage.isValid())
		_renderImage(*pMinecraft, isSelected() ? m_iconImage : m_downIconImage);
	else
		_renderMessage(*pMinecraft);
}
