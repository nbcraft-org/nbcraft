#include "DynamicTexture.hpp"
#include "world/item/Item.hpp"
#include "client/resources/Resource.hpp"
#include "client/app/Minecraft.hpp"

CompassTexture::CompassTexture(Minecraft* mc) : DynamicTexture(Item::compass->m_icon), m_pMinecraft(mc), m_rot(0), m_rota(0)
{
    m_textureId = 1;

    TextureData* texture = mc->m_pTextures->getTextureData(C_ITEMS_NAME, true);

    for (int i = 0; i < 256; i++)
    {
        int x = i % 16;
        int y = i / 16;

        int texX = (m_textureIndex % 16) * 16 + x;
        int texY = (m_textureIndex / 16) * 16 + y;

        m_data[i] = texture->getData()[texY * texture->m_imageData.m_width + texX];
    }
}

void CompassTexture::tick()
{
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

    float rott = 0.0f;
    float rotd;
    float sin;
    if (m_pMinecraft->m_pLevel && m_pMinecraft->m_pLocalPlayer)
    {
        rotd = m_pMinecraft->m_pLevel->getSharedSpawnPos().x - m_pMinecraft->m_pLocalPlayer->m_pos.x;
        sin = m_pMinecraft->m_pLevel->getSharedSpawnPos().z - m_pMinecraft->m_pLocalPlayer->m_pos.z;
        rott = (m_pMinecraft->m_pLocalPlayer->m_rot.y - 90.0f) * M_PI / 180.0f - Mth::atan2(sin, rotd);
        if (m_pMinecraft->m_pLevel->m_pDimension->m_bFoggy) {
            rott = Mth::random() * M_PI * 2.0f;
        }
    }

    for (rotd = rott - m_rot; rotd < -M_PI; rotd += M_PI * 2) {
    }

    while (rotd >= M_PI)
        rotd -= 2 * M_PI;

    if (rotd < -1.0f)
        rotd = -1.0f;

    if (rotd > 1.0f)
        rotd = 1.0f;

    m_rota += rotd * 0.1f;
    m_rota *= 0.8f;
    m_rot += m_rota;
    sin = Mth::sin(m_rot);
    float cos = Mth::cos(m_rot);

    int d;
    int x;
    int y;
    int i;
    int r;
    int g;
    int b;
    short a;
    for (d = -4; d <= 4; ++d) {
        x = (int)(8.5f + cos * (float)d * 0.3f);
        y = (int)(7.5f - sin * (float)d * 0.3f * 0.5f);
        i = y * 16 + x;
        r = 100;
        g = 100;
        b = 100;
        a = 255;
        //if (m_bAnaglyph3d)
        //{
        //    rr = (r * 30 + g * 59 + b * 11) / 100;
        //    gg = (r * 30 + g * 70) / 100;
        //    bb = (r * 30 + b * 70) / 100;
        //    r = rr;
        //    g = gg;
        //    b = bb;
        //}

        m_pixels[i * 4 + 0] = r;
        m_pixels[i * 4 + 1] = g;
        m_pixels[i * 4 + 2] = b;
        m_pixels[i * 4 + 3] = a;
    }

    for (d = -8; d <= 16; ++d) {
        x = (int)(8.5f + sin * (float)d * 0.3f);
        y = (int)(7.5f + cos * (float)d * 0.3f * 0.5f);
        i = y * 16 + x;
        r = d >= 0 ? 255 : 100;
        g = d >= 0 ? 20 : 100;
        b = d >= 0 ? 20 : 100;
        a = 255;
        //if (m_bAnaglyph3d)
        //{
        //    rr = (r * 30 + g * 59 + b * 11) / 100;
        //    gg = (r * 30 + g * 70) / 100;
        //    bb = (r * 30 + b * 70) / 100;
        //    r = rr;
        //    g = gg;
        //    b = bb;
        //}

        m_pixels[i * 4 + 0] = r;
        m_pixels[i * 4 + 1] = g;
        m_pixels[i * 4 + 2] = b;
        m_pixels[i * 4 + 3] = a;
    }
}
