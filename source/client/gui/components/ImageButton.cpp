#include "ImageButton.hpp"
#include "renderer/ShaderConstants.hpp"

void ImageButton::_init()
{
	m_bScaleOnPress = false;
}

ImageButton::ImageButton(const std::string& text)
	: Button(text)
{
	_init();

	setupDefault();
}

ImageButton::ImageButton(const std::string& text, const ImageDef& imageDef)
	: Button(text)
	, m_image(imageDef)
{
	_init();

	setupDefault();
}

void ImageButton::_renderImage(Minecraft& mc, bool isPressed)
{
    Textures& textures = *mc.m_pTextures;
    Tesselator& t = Tesselator::instance;

    bool bSecondImage = isSecondImage(isPressed);

    TextureData* pTextureData = textures.loadAndBindTexture(m_image.name);
    if (m_image.name.empty() || !pTextureData)
        return;

    Color color = isEnabled() ? 0xFFFFFFFF : 0xFF808080;

    t.begin(4);
    t.color(color);

    float v14 = (m_image.width  * 0.5f) * ((float)m_width  / m_image.width);
    float v15 = (m_image.height * 0.5f) * ((float)m_height / m_image.height);
    float v16 = (float)(m_xPos + m_image.xOffset) + v14;
    float v17 = (float)(m_yPos + m_image.yOffset) + v15;

    if (m_bScaleOnPress && isPressed)
    {
        v14 = v14 * 0.95f;
        v15 = v15 * 0.95f;
    }

    if (m_image.bUseCustomUV)
    {
        int v20 = bSecondImage ? m_image.uv.w : 0;
        int v22 = m_image.uv.x + v20;
        float v24 = (float)v22 / pTextureData->m_imageData.m_width;
        int uvWidth = m_image.uv.w;
        if (bSecondImage)
            uvWidth *= 2;
        float v26 = (float)(m_image.uv.x + uvWidth     ) / pTextureData->m_imageData.m_width;
        float v28 = (float)(m_image.uv.y + m_image.uv.h) / pTextureData->m_imageData.m_height;
        t.vertexUV(v16 - v14, v17 - v15, m_blitOffset, v24, (float)m_image.uv.y / pTextureData->m_imageData.m_height);
        t.vertexUV(v16 - v14, v17 + v15, m_blitOffset, v24, v28);
        t.vertexUV(v16 + v14, v17 + v15, m_blitOffset, v26, v28);
        t.vertexUV(v16 + v14, v17 - v15, m_blitOffset, v26, (float)m_image.uv.y / pTextureData->m_imageData.m_height);
    }
    else
    {
        t.vertexUV(v16 - v14, v17 - v15, m_blitOffset, 0.0f, 0.0f);
        t.vertexUV(v16 - v14, v17 + v15, m_blitOffset, 0.0f, 1.0f);
        t.vertexUV(v16 + v14, v17 + v15, m_blitOffset, 1.0f, 1.0f);
        t.vertexUV(v16 + v14, v17 - v15, m_blitOffset, 1.0f, 0.0f);
    }

    t.draw(m_materials.ui_textured_and_glcolor);
}

void ImageButton::_renderMessage(Minecraft& mc, bool isPressed)
{
    Font& font = *mc.m_pFont;
    const std::string& message = getMessage();

    Color color = 0xFFA0A0A0;
    float yOffset = 0.0f;

    if (isEnabled())
    {
        if (isPressed || isSelected())
        {
            color = 0xA0FFFF; // @PARITY-PE: 0xFFFFA0 on PE, which is blue, probably an endianness issue and should be yellow
            yOffset = 1.0f;
        }
        else
        {
            color = 0xE0E0E0;
        }
    }

    drawCenteredString(font, message, m_xPos + (m_width / 2), m_yPos + m_image.yOffset + yOffset, color);
}

void ImageButton::renderBg(Minecraft* pMinecraft, const MenuPointer& pointer)
{
}

void ImageButton::render(Minecraft* pMinecraft, const MenuPointer& pointer)
{
	if (!isVisible()) return;

	if (!pMinecraft->m_pScreen->doElementTabbing())
		setSelected(isHovered(pMinecraft, pointer));

	if (m_color.a == 0.0f)
		return;

	currentShaderColor = m_color;

    renderBg(pMinecraft, pointer);

    bool shouldDrawPressed = isSelected() && isHovered(pMinecraft, pointer);

	_renderImage(*pMinecraft, shouldDrawPressed);
    _renderMessage(*pMinecraft, shouldDrawPressed);
}

void ImageButton::setImageDef(const ImageDef& imageDef, bool setSize)
{
	m_image = imageDef;

	if (setSize)
	{
		m_width = imageDef.width;
		m_height = imageDef.height;
	}
}

void ImageButton::setupDefault()
{
	m_width = 48;
	m_height = 48;
	m_bScaleOnPress = true;
}
