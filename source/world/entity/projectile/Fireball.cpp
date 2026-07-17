#include "Fireball.hpp"
#include "world/level/Level.hpp"

void Fireball::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::largeFireball;
    m_renderType = RENDER_FIREBALL;
    setSize(1.0f, 1.0f);
}

Fireball::Fireball(Level* pLevel) : Snowball(pLevel)
{
    _init();
}

Fireball::Fireball(Level* pLevel, Mob* pMob, Vec3 pos) : Snowball(pLevel)
{
    _init();

    m_owner = pMob;
    m_bIsPlayerOwned = m_owner->isPlayer();
    moveTo(Vec3(pMob->m_pos.x, pMob->m_pos.y, pMob->m_pos.z), pMob->m_rot);

    setPos(m_pos);

    m_vel = Vec3::ZERO;
    pos.x += m_random.nextGaussian() * 0.4f;
    pos.y += m_random.nextGaussian() * 0.4f;
    pos.z += m_random.nextGaussian() * 0.4f;

    float len = pos.length();
    m_powerVel.x = pos.x / len * 0.1f;
    m_powerVel.y = pos.y / len * 0.1f;
    m_powerVel.z = pos.z / len * 0.1f;
}

void Fireball::tick()
{
    m_fireTicks = 10;
    Snowball::tick();
}

void Fireball::onHit()
{
    if (!m_pLevel->m_bIsClientSide)
        m_pLevel->explode(nullptr, m_pos, 1.0f, true);
}

void Fireball::readAdditionalSaveData(const CompoundTag& tag)
{
    Snowball::readAdditionalSaveData(tag);
    m_powerVel = m_vel;
}

bool Fireball::hurt(Entity* pCulprit, int damage)
{
    markHurt();
    if (pCulprit)
    {
        Vec3 ang = pCulprit->getLookAngle();
        if (ang != Vec3::ZERO)
        {
            m_vel = ang;
            m_powerVel = m_vel * 0.1f;
        }

        return true;
    }
    else
    {
        return false;
    }
}
