#include "ThrownEgg.hpp"
#include "Mob.hpp"
#include "Chicken.hpp"
#include "world/level/Level.hpp"
#include "nbt/CompoundTag.hpp"

void ThrownEgg::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::thrownEgg;
    m_renderType = RENDER_THROWN_EGG;
    m_tilePos = TilePos(-1, -1, -1);
	m_lastTile = 0;
	m_lastTileData = 0; 
	m_bInGround = false; 
	m_bIsPlayerOwned = false; 
	m_life = 0; 
	m_flightTime = 0; 
	m_shakeTime = 0; ; 
	m_owner = nullptr; 
}

ThrownEgg::ThrownEgg(Level* pLevel) : Entity(pLevel) 
{
	_init();
	setSize(0.25f, 0.25f);
}

ThrownEgg::ThrownEgg(Level* pLevel, Mob* pMob) : Entity(pLevel) 
{
	_init();

	m_owner = pMob;
	setSize(0.25f, 0.25f);
	m_bIsPlayerOwned = m_owner->isPlayer();
	moveTo(Vec3(pMob->m_pos.x, pMob->m_pos.y + pMob->getHeadHeight(), pMob->m_pos.z), Vec2(pMob->m_rot.y, pMob->m_rot.x));

	m_pos.x -= Mth::cos(m_rot.y / 180.0f * M_PI) * 0.16f;
	m_pos.y -= 0.1f;
	m_pos.z -= Mth::sin(m_rot.y / 180.0f * M_PI) * 0.16f;
	setPos(m_pos);
    constexpr float f = 0.4f;
	m_vel.x = -Mth::sin(m_rot.y / 180.0f * M_PI) * Mth::cos(m_rot.x / 180.0f * M_PI) * f;
	m_vel.z = Mth::cos(m_rot.y / 180.0f * M_PI) * Mth::cos(m_rot.x / 180.0f * M_PI) * f;
	m_vel.y = -Mth::sin(m_rot.x / 180.0f * M_PI) * f;
	shoot(m_vel, 1.5f, 1.0f);
}

ThrownEgg::ThrownEgg(Level* pLevel, const Vec3& pos) 
{
    _init();

	m_life = 0;
	setSize(0.25f, 0.25f);
	setPos(m_pos);
	//eyeHeight = 0.0F;
}

void ThrownEgg::shoot(Vec3 vel, float speed, float r)
{
    float f = Mth::sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
    vel /= f;
	vel.x += sharedRandom.nextGaussian() * 0.0075f * r;
	vel.y += sharedRandom.nextGaussian() * 0.0075f * r;
	vel.z += sharedRandom.nextGaussian() * 0.0075f * r;
    vel *= speed;
    m_vel = vel;
    _lerpMotion(vel);
	m_life = 0;
}

void ThrownEgg::_lerpMotion(const Vec3& vel)
{
    float len = vel.length();
    m_oRot.y = m_rot.y = Mth::atan2(vel.x, vel.z) * 180.0f / M_PI;
    m_oRot.x = m_rot.x = Mth::atan2(vel.y, len) * 180.0f / M_PI;
}

void ThrownEgg::_lerpMotion2(const Vec3& vel)
{
    if (m_oRot.x == 0.0f && m_oRot.y == 0.0f)
    {
        return _lerpMotion(vel);
    }
}

void ThrownEgg::lerpMotion(const Vec3& vel)
{
    m_vel = vel;

    _lerpMotion2(vel);
}

bool ThrownEgg::shouldRenderAtSqrDistance(float distSqr) const
{
	float avgSide = (this->m_bbWidth + m_bbHeight + m_bbWidth) / 3.0f;
	float d = avgSide * 4;
	d *= 64.0;
	return distSqr < d * d;
}

void ThrownEgg::tick() 
{
    m_posPrev = m_pos;
    Entity::tick();
    if (m_shakeTime > 0)
        --m_shakeTime;

    if (m_bInGround)
    {
        if (m_pLevel->getTile(m_tilePos) == m_lastTile)
        {
            ++m_life;
            if (m_life == 1200)
            {
                remove();
            }

            return;
        }

        m_bInGround = false;
        m_vel.x *= sharedRandom.nextFloat() * 0.2f;
        m_vel.y *= sharedRandom.nextFloat() * 0.2f;
        m_vel.z *= sharedRandom.nextFloat() * 0.2f;
        m_life = 0;
        m_flightTime = 0;
    }
    else {
        ++m_flightTime;
    }
    Vec3 future_pos = m_pos + m_vel;
    HitResult hit_result = m_pLevel->clip(m_pos, future_pos);
    if (hit_result.isHit())
    {
        future_pos = hit_result.m_hitPos;
    }

    Entity* hit_ent = nullptr;
    AABB hitbox = m_hitbox;
    hitbox.expand(m_vel.x, m_vel.y, m_vel.z).grow(1.0f);
    EntityVector entities = m_pLevel->getEntities(this, hitbox);

    float max_dist = 0.0f;
    constexpr float var10 = 0.3f;
    for (EntityVector::iterator it = entities.begin(); it != entities.end(); it++)
    {
        Entity* ent = *it;
        if (ent->m_bCollision && (ent != m_owner || m_flightTime >= 5))
        {
            AABB aabb = ent->m_hitbox;
            aabb.grow(var10);
            HitResult hit = aabb.clip(m_pos, future_pos);
            if (hit.isHit())
            {
                float distance = m_pos.distanceTo(hit.m_hitPos);
                if (distance < max_dist || max_dist == 0.0f)
                {
                    hit_ent = ent;
                    max_dist = distance;
                }
            }
        }
    }

    if (hit_ent != nullptr)
    {
        hit_result = HitResult(hit_ent);
    }

    if (hit_result.isHit())
    {
        if (hit_result.m_pEnt != nullptr)
        {
            hit_result.m_pEnt->hurt(m_owner, 0);
        }
        if (!m_pLevel->m_bIsClientSide && sharedRandom.nextInt(8) == 0)
        {
            int j = 1;
            if (sharedRandom.nextInt(32) == 0)
                j = 4;

            for (int l = 0; l < j; l++)
            {
                Chicken* chicken = new Chicken(m_pLevel);
                chicken->moveTo(m_pos, 0.0f);
                m_pLevel->addEntity(chicken);
            }
        }
        for (int k = 0; k < 8; k++)
        {
            m_pLevel->addParticle("snowballpoof", m_pos, 0);
        }
        remove();
    }

    m_pos += m_vel;
    float var17 = m_vel.length();
    m_rot.y = Mth::atan2(m_vel.x, m_vel.z) * 180.0f / M_PI;

    for (m_rot.x = Mth::atan2(m_vel.y, var17) * 180.0f / M_PI; m_rot.x - m_oRot.x < -180.0f; m_oRot.x -= 360.0f);

    while (m_rot.x - m_oRot.x >= 180.0f)
    {
        m_oRot.x += 360.0f;
    }

    while (m_rot.y - m_oRot.y < -180.0f)
    {
        m_oRot.y -= 360.0f;
    }

    while (m_rot.y - m_oRot.y >= 180.0f)
    {
        m_oRot.y += 360.0f;
    }

    m_rot.x = m_oRot.x + (m_rot.x - m_oRot.x) * 0.2f;
    m_rot.y = m_oRot.y + (m_rot.y - m_oRot.y) * 0.2f;
    float dampening = 0.99f;
    if (isInWater())
    {
        for (int var19 = 0; var19 < 4; ++var19)
        {
            constexpr float var20 = 0.25f;
            m_pLevel->addParticle("bubble", m_pos - m_vel * var20, m_pos * 1);
        }

        dampening = 0.8f;
    }

    m_vel *= dampening;
    m_vel.y -= 0.03f;
    setPos(m_pos);
}

void ThrownEgg::addAdditionalSaveData(CompoundTag& tag) const
{
    tag.putInt16("xTile", m_tilePos.x);
    tag.putInt16("yTile", m_tilePos.y);
    tag.putInt16("zTile", m_tilePos.z);
    tag.putInt8("inTile", m_lastTile);
    tag.putInt8("shake", m_shakeTime);
    tag.putBoolean("inGround", m_bInGround);
    tag.putBoolean("player", m_bIsPlayerOwned);
}

void ThrownEgg::readAdditionalSaveData(const CompoundTag& tag)
{
    m_tilePos.x = tag.getInt16("xTile");
    m_tilePos.y = tag.getInt16("yTile");
    m_tilePos.z = tag.getInt16("zTile");
    m_lastTile = tag.getInt8("inTile") & 255;
    m_shakeTime = tag.getInt8("shake") & 255;
    m_bInGround = tag.getBoolean("inGround");
    m_bIsPlayerOwned = tag.getBoolean("player");
}

Entity::AuxValue ThrownEgg::getAuxValue() const
{
    return m_owner ? m_owner->m_EntityID : 0;
}

void ThrownEgg::setAuxValue(Entity::AuxValue value)
{
    Entity* pOwner = m_pLevel->getEntity(value);
    if (pOwner && pOwner->isMob())
        m_owner = (Mob*)pOwner;
}
