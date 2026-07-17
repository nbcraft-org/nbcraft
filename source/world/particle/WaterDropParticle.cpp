#include "Particle.hpp"
#include "world/tile/LiquidTile.hpp"
#include "world/level/Level.hpp"

WaterDropParticle::WaterDropParticle(Level* level, const Vec3& pos) :
	Particle(level, pos, Vec3::ZERO)
{
	m_vel.x *= 0.3;
	m_vel.y = Mth::random() * 0.2f + 0.1f;
	m_vel.z *= 0.3;
	m_rCol = m_gCol = m_bCol = 1.0f;
	m_tex = 19 + m_random.nextInt(4);
	setSize(0.01f, 0.01f);
	m_gravity = 0.06f;
	m_lifetime = (int)(8.0 / (Mth::random() * 0.8 + 0.2));
}

void WaterDropParticle::tick()
{
	m_oPos = m_pos;
    m_vel.y -= m_gravity;
    move(m_vel);
    m_vel *= 0.98;
	if (m_lifetime-- <= 0)
		remove();

    if (m_bOnGround)
    {
        if (Mth::random() < 0.5)
        {
            remove();
        }

        m_vel.x *= 0.7;
        m_vel.z *= 0.7;
    }

    Material* m = m_pLevel->getMaterial(m_pos);
    if (m->isLiquid() || m->isSolid())
    {
        float y0 = Mth::floor(m_pos.y) + 1 - LiquidTile::getHeight(m_pLevel->getData(m_pos));
        if (m_pos.y < y0)
        {
            remove();
        }
    }
}
