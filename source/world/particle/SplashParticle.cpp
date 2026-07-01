#include "Particle.hpp"

SplashParticle::SplashParticle(Level* level, const Vec3& pos, const Vec3& dir) :
	WaterDropParticle(level, pos)
{
    m_gravity = 0.04F;
    ++m_tex;
    if (dir.y == 0.0 && (dir.x != 0.0 || dir.z != 0.0)) {
        m_vel.x = dir.x;
        m_vel.y = dir.y + 0.1;
        m_vel.z = dir.z;
    }
}