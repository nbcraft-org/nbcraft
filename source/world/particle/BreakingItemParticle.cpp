#include "Particle.hpp"
#include "world/level/Level.hpp"

BreakingItemParticle::BreakingItemParticle(Level* level, const Vec3& pos, Item* item) :
	Particle(level, pos, Vec3::ZERO)
{
	field_DC = item->getIcon(nullptr); // this.tex
	m_rCol = m_gCol = m_bCol = 1.0F;
	field_F4 = Tile::snow->field_28; // this.gravity = Tile.snow.gravity;
	field_F0 /= 2.0F; // this.size
}

int BreakingItemParticle::getParticleTexture()
{
	return PT_ITEM;
}

void BreakingItemParticle::render(Tesselator& t, float a, float xa, float ya, float za, float xa2, float za2)
{
    float u0 = (float(field_DC % 16) + field_E0 / 4.0f) / 16.0f; // field_E0 is this.uo
    float u1 = u0 + 0.015609375f;
    float v0 = (float(field_DC / 16) + field_E4 / 4.0f) / 16.0f; // field_E4 is this.vo
    float v1 = v0 + 0.015609375f;
    float r = 0.1F * field_F0;
    float x = float(m_oPos.x + (m_pos.x - m_oPos.x) * a - xOff);
    float y = float(m_oPos.y + (m_pos.y - m_oPos.y) * a - yOff);
    float z = float(m_oPos.z + (m_pos.z - m_oPos.z) * a - zOff);
    float br = getBrightness(a);
    t.color(br * m_rCol, br * m_gCol, br * m_bCol);
    t.vertexUV(x - xa * r - xa2 * r, y - ya * r, z - za * r - za2 * r, u0, v1);
    t.vertexUV(x - xa * r + xa2 * r, y + ya * r, z - za * r + za2 * r, u0, v0);
    t.vertexUV(x + xa * r + xa2 * r, y + ya * r, z + za * r + za2 * r, u1, v0);
    t.vertexUV(x + xa * r - xa2 * r, y - ya * r, z + za * r - za2 * r, u1, v1);
}