#include "Particle.hpp"

ItemParticle::ItemParticle(Level* level, const Vec3& pos, const Item* item) :
	Particle(level, pos, Vec3::ZERO)
{
	m_tex = item->getIcon(0);
    m_gravity = Tile::snow->m_gravity;
	m_rCol = m_gCol = m_bCol = 1.0f;
	m_size /= 2.0f;
}

int ItemParticle::getParticleTexture()
{
    return PT_PARTICLES2;
}

void ItemParticle::render(Tesselator& t, float f, float a, float b, float c, float d, float e)
{
    float MagicValue = 0.015609375f;
    float fa = (m_tex % 16 + m_uo/ 4.0F) / 16.0F;
    float ga = fa + MagicValue;
    float ha = (m_tex / 16 + m_vo / 4.0F) / 16.0F;
    float ia = ha + MagicValue;
    float ja = 0.1F * m_size;
    float PosX = Mth::Lerp(m_oPos.x, m_pos.x, f) - xOff;
    float PosY = Mth::Lerp(m_oPos.y, m_pos.y, f) - yOff;
    float PosZ = Mth::Lerp(m_oPos.z, m_pos.z, f) - zOff;
    float fBright = getBrightness(f);
    t.color(fBright * m_rCol, fBright * m_gCol, fBright * m_bCol);
    t.vertexUV(PosX - a * ja - d * ja, PosY - b * ja, PosZ - c * ja - e * ja, fa, ia);
    t.vertexUV(PosX - a * ja + d * ja, PosY + b * ja, PosZ - c * ja + e * ja, fa, ha);
    t.vertexUV(PosX + a * ja + d * ja, PosY + b * ja, PosZ + c * ja + e * ja, ga, ha);
    t.vertexUV(PosX + a * ja - d * ja, PosY - b * ja, PosZ + c * ja - e * ja, ga, ia);
}
