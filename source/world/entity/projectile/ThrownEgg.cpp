#include "ThrownEgg.hpp"
#include "world/entity/Chicken.hpp"
#include "world/level/Level.hpp"

void ThrownEgg::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::thrownEgg;
    m_renderType = RENDER_THROWN_EGG;
    setSize(0.25f, 0.25f);
}

ThrownEgg::ThrownEgg(Level* pLevel)
    : Snowball(pLevel) 
{
	_init();
}

ThrownEgg::ThrownEgg(Level* pLevel, Mob* pMob)
    : Snowball(pLevel, pMob) 
{
	_init();
}

ThrownEgg::ThrownEgg(Level* pLevel, const Vec3& pos, bool isPlayerOwned)
    : Snowball(pLevel, pos, isPlayerOwned)
{
    _init();
}

void ThrownEgg::onHit()
{
    if (!m_pLevel->m_bIsClientSide && m_random.nextInt(8) == 0)
    {
        int j = 1;
        if (m_random.nextInt(32) == 0)
            j = 4;

        for (int l = 0; l < j; l++)
        {
            Chicken* chicken = new Chicken(m_pLevel);
            chicken->moveTo(m_pos, 0.0f);
            m_pLevel->addEntity(chicken);
        }
    }
    Snowball::onHit();
}
