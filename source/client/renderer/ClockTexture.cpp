#include "DynamicTexture.hpp"
#include "world/item/Item.hpp"
#include "client/app/Minecraft.hpp"

ClockTexture::ClockTexture(Minecraft* minecraft)
    : DynamicTexture(Item::clock->m_icon)
    , m_pMinecraft(minecraft)
    , m_rot(0)
    , m_rota(0)
{
    m_dialData = minecraft->m_pTextures->getTextureData("misc/dial.png", true)->getData();
    m_textureId = 1;

    TextureData* texture = minecraft->m_pTextures->getTextureData(C_ITEMS_NAME, true);

    for (int i = 0; i < 256; i++)
    {
        int x = i % 16;
        int y = i / 16;

        int texX = (m_textureIndex % 16) * 16 + x;
        int texY = (m_textureIndex / 16) * 16 + y;

        m_data[i] = texture->getData()[texY * texture->m_imageData.m_width + texX];
    }
}

void ClockTexture::tick()
{
    float rott = 0.0f;
    if (m_pMinecraft->m_pLevel && m_pMinecraft->m_pLocalPlayer)
    {
        float time = m_pMinecraft->m_pLevel->getTimeOfDay(1.0f);
        rott = (float)(-time * M_PI * 2.0f);
        if (m_pMinecraft->m_pLevel->m_pDimension->m_bFoggy)
            rott = Mth::random() * M_PI * 2.0f;
    }

    float rotd;
    for (rotd = rott - m_rot; rotd < -M_PI; rotd += 2 * M_PI) {
    }

    while (rotd >= M_PI)
        rotd -= 2 * M_PI;

    if (rotd < -1.0f)
        rotd = -1.0f;

    if (rotd > 1.0f)
        rotd = 1.0f;

    m_rota += rotd * 0.1f;
    m_rota *= 0.8;
    m_rot += m_rota;
    float sin = Mth::sin(m_rot);
    float cos = Mth::cos(m_rot);

    for (int i = 0; i < 256; ++i)
    {
#if MC_ENDIANNESS_BIG
        int a = m_data[i] >> 0  & 255;
        int r = m_data[i] >> 8  & 255;
        int g = m_data[i] >> 16 & 255;
        int b = m_data[i] >> 24 & 255;
#else // MC_ENDIANNESS_LITTLE
        int a = m_data[i] >> 24 & 255;
        int r = m_data[i] >> 16 & 255;
        int g = m_data[i] >> 8  & 255;
        int b = m_data[i] >> 0  & 255;
#endif
        //@NOTE Temporary fix
        std::swap(r, b);
        if (r == b && g == 0 && b > 0)
        {
            float xo = -((float)(i % 16) / 15.0f - 0.5f);
            float yo = (float)(i / 16) / 15.0f - 0.5f;
            int br = r;
            int x = (int)((xo * cos + yo * sin + 0.5f) * 16.0f);
            int y = (int)((yo * cos - xo * sin + 0.5f) * 16.0f);
            int j = (x & 15) + (y & 15) * 16;
#if MC_ENDIANNESS_BIG
            a =  m_dialData[j] >> 0  & 255;
            r = (m_dialData[j] >> 8  & 255) * r / 255;
            g = (m_dialData[j] >> 16 & 255) * br / 255;
            b = (m_dialData[j] >> 24 & 255) * br / 255;
#else // MC_ENDIANNESS_LITTLE
            a =  m_dialData[j] >> 24 & 255;
            r = (m_dialData[j] >> 16 & 255) * r / 255;
            g = (m_dialData[j] >> 8  & 255) * br / 255;
            b = (m_dialData[j] >> 0  & 255) * br / 255;
#endif
            std::swap(r, b);
        }

        if (m_bAnaglyph3d)
        {
            int rr = (r * 30 + g * 59 + b * 11) / 100;
            int gg = (r * 30 + g * 70) / 100;
            int bb = (r * 30 + b * 70) / 100;
            r = rr;
            g = gg;
            b = bb;
        }

        m_pixels[i * 4 + 0] = r;
        m_pixels[i * 4 + 1] = g;
        m_pixels[i * 4 + 2] = b;
        m_pixels[i * 4 + 3] = a;
    }
}
