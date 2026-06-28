#include "SlimeRenderer.hpp"
#include "renderer/ShaderConstants.hpp"

SlimeRenderer::SlimeRenderer(Model* pModel, Model* pArmor, float f) : MobRenderer(pModel, f)
{
    m_armor = pArmor;
}

SlimeRenderer::~SlimeRenderer()
{
}

int SlimeRenderer::prepareArmor(const Mob& mob, int layer, float a)
{
    if (layer == 0)
    {
        setArmor(m_armor);
#if MCE_GFX_API_OGL && !defined(FEATURE_GFX_SHADERS)
        glEnable(GL_RESCALE_NORMAL);
#endif
        return 1;
    }
    else
    {
        if (layer == 1)
        {
            currentShaderColor = Color::WHITE;
        }
        return 0;
    }
}

void SlimeRenderer::scale(const Mob& mob, Matrix& matrix, float a)
{
    const Slime& slime = (const Slime&)mob;
    float ss = (slime.m_oSquish + (slime.m_squish - slime.m_oSquish) * a) / (float(slime.m_size) * 0.5f + 1.0f);
    float w = 1.0f / (ss + 1.0f);
    float s = float(slime.m_size);
    matrix.scale(Vec3(w * s, 1.0f / w * s, w * s));
}
