#include "LargeImageButton.hpp"
#include "renderer/ShaderConstants.hpp"

void LargeImageButton::_init()
{
    m_unknown = 1.0f;
}

LargeImageButton::LargeImageButton(const std::string& text)
    : ImageButton(text)
{
    _init();

    setupDefault();
}

LargeImageButton::LargeImageButton(const std::string& text, const ImageDef& imageDef)
    : ImageButton(text, imageDef)
{
    _init();

    setupDefault();
}

void LargeImageButton::_renderImage(Minecraft& mc, bool isPressed)
{
    Textures& textures = *mc.m_pTextures;
    Tesselator& t = Tesselator::instance;

    TextureData* pTextureData = textures.loadAndBindTexture(m_image.name);
    if (m_image.name.empty() || !pTextureData)
        return;

    Color color = isEnabled() ? 0xFFFFFFFF : 0xFF808080;

    t.begin(4);
    t.color(color);

    float width2 = m_image.width * 0.5f;
    float height2 = m_image.height * 0.5f;
    float v14 = (m_image.width * 0.5f) * ((float)m_width / m_image.width);
    float v15 = (m_image.height * 0.5f) * ((float)m_height / m_image.height);
    float v16 = (float)(m_xPos + m_image.xOffset) + v14;
    float v17 = (float)(m_yPos + m_image.yOffset) + v15;

    m_unknown = isPressed && mc.useTouchscreen() ? Mth::Max(0.95f, m_unknown - 0.025f) : Mth::Min(1.0f, m_unknown + 0.025f);

    v14 = width2 * m_unknown;
    v15 = height2 * m_unknown;

    if (m_image.bUseCustomUV)
    {
        int v20 = isPressed ? m_image.uv.w : 0;
        int v22 = m_image.uv.x + v20;
        float v24 = (float)v22 / pTextureData->m_imageData.m_width;
        int uvWidth = m_image.uv.w;
        if (isPressed)
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

void LargeImageButton::_renderMessage(Minecraft& mc, bool isPressed)
{
    Font& font = *mc.m_pFont;
    const std::string& message = getMessage();

    Color color = 0xFFA0A0A0;

    if (isEnabled())
    {
        if (isPressed || isSelected())
            color = 0xA0FFFF; // @PARITY-PE: 0xFFFFA0 on PE, which is blue, probably an endianness issue and should be yellow
        else
            color = 0xE0E0E0;
    }

    drawCenteredString(font, message, m_xPos + (m_width / 2), m_yPos + m_image.yOffset + 11, color);
}

void LargeImageButton::setupDefault()
{
    m_width = 72;
    m_height = 72;
    m_unknown = 1.0f;
}
