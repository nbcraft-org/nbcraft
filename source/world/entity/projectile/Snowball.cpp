#include "Snowball.hpp"
#include "world/entity/Mob.hpp"
#include "world/level/Level.hpp"
#include "nbt/CompoundTag.hpp"

void Snowball::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::snowball;
    m_renderType = RENDER_SNOWBALL;
    setSize(0.25f, 0.25f);

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

Snowball::Snowball(Level* pLevel)
    : Entity(pLevel) 
{
    _init();
}

Snowball::Snowball(Level* pLevel, Mob* pMob)
    : Entity(pLevel) 
{
    _init();

    m_owner = pMob;
    m_bIsPlayerOwned = m_owner->isPlayer();
    moveTo(Vec3(pMob->m_pos.x, pMob->m_pos.y + pMob->getHeadHeight(), pMob->m_pos.z), pMob->m_rot);

    m_pos.x -= Mth::cos(m_rot.yaw / 180.0f * M_PI) * 0.16f;
    m_pos.y -= 0.1f;
    m_pos.z -= Mth::sin(m_rot.yaw / 180.0f * M_PI) * 0.16f;
    setPos(m_pos);

    constexpr float f = 0.4f;
    m_vel.x = -Mth::sin(m_rot.yaw / 180.0f * M_PI) * Mth::cos(m_rot.pitch / 180.0f * M_PI) * f;
    m_vel.z = Mth::cos(m_rot.yaw / 180.0f * M_PI) * Mth::cos(m_rot.pitch / 180.0f * M_PI) * f;
    m_vel.y = -Mth::sin(m_rot.pitch / 180.0f * M_PI) * f;
    shoot(m_vel, 1.5f, 1.0f);
}

Snowball::Snowball(Level* pLevel, const Vec3& pos, bool isPlayerOwned)
    : Entity(pLevel)
{
    _init();

    setPos(pos);

    m_bIsPlayerOwned = isPlayerOwned;
}

void Snowball::shoot(Vec3 vel, float speed, float r) 
{
    float f = Mth::sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
    vel /= f;
    vel.x += m_random.nextGaussian() * 0.0075f * r;
    vel.y += m_random.nextGaussian() * 0.0075f * r;
    vel.z += m_random.nextGaussian() * 0.0075f * r;
    vel *= speed;
    m_vel = vel;
    _lerpMotion(vel);
    m_life = 0;
}

void Snowball::_lerpMotion(const Vec3& vel)
{
    float len = Vec2(vel.x, vel.z).length();
    m_oRot.yaw = m_rot.yaw = Mth::atan2(vel.x, vel.z) * 180.0f / M_PI;
    m_oRot.pitch = m_rot.pitch = Mth::atan2(vel.y, len) * 180.0f / M_PI;
}

void Snowball::_lerpMotion2(const Vec3& vel)
{
    if (m_oRot == Rot2::ZERO)
    {
        return _lerpMotion(vel);
    }
}

void Snowball::lerpMotion(const Vec3& vel)
{
    m_vel = vel;

    _lerpMotion2(vel);
}

bool Snowball::shouldRenderAtSqrDistance(float distSqr) const 
{
    float avgSide = (this->m_bbWidth + m_bbHeight + m_bbWidth) / 3.0f;
    float d = avgSide * 4;
    d *= 64.0;
    return distSqr < d * d;
}

void Snowball::tick() 
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
        m_vel.x *= m_random.nextFloat() * 0.2f;
        m_vel.y *= m_random.nextFloat() * 0.2f;
        m_vel.z *= m_random.nextFloat() * 0.2f;
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
        onHit();
        remove();
    }

    m_pos += m_vel;
    float lengthVel2 = Vec2(m_vel.x, m_vel.z).length();
    m_rot.yaw = Mth::atan2(m_vel.x, m_vel.z) * 180.0f / M_PI;

    for (m_rot.pitch = Mth::atan2(m_vel.y, lengthVel2) * 180.0f / M_PI; m_rot.pitch - m_oRot.pitch < -180.0f; m_oRot.pitch -= 360.0f);

    while (m_rot.pitch - m_oRot.pitch >= 180.0f)
    {
        m_oRot.pitch += 360.0f;
    }

    while (m_rot.yaw - m_oRot.yaw < -180.0f)
    {
        m_oRot.yaw -= 360.0f;
    }

    while (m_rot.yaw - m_oRot.yaw >= 180.0f)
    {
        m_oRot.yaw += 360.0f;
    }

    m_rot = m_oRot + (m_rot - m_oRot) * 0.2f;
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

    if (m_powerVel != Vec3::ZERO)
        m_vel += m_powerVel;
    m_vel *= dampening;
    m_vel.y -= 0.03f;
    setPos(m_pos);
}

void Snowball::onHit()
{
    for (int i = 0; i < 8; ++i)
        m_pLevel->addParticle("snowballpoof", m_pos);
}

void Snowball::addAdditionalSaveData(CompoundTag& tag) const
{
    tag.putInt16("xTile", m_tilePos.x);
    tag.putInt16("yTile", m_tilePos.y);
    tag.putInt16("zTile", m_tilePos.z);
    tag.putInt8("inTile", m_lastTile);
    tag.putInt8("shake", m_shakeTime);
    tag.putBoolean("inGround", m_bInGround);
    //Why are we saving this? No idea, this is useful only for arrows, which don't disappear upon hitting a tile
    tag.putBoolean("player", m_bIsPlayerOwned);
}

void Snowball::readAdditionalSaveData(const CompoundTag& tag)
{
    m_tilePos.x = tag.getInt16("xTile");
    m_tilePos.y = tag.getInt16("yTile");
    m_tilePos.z = tag.getInt16("zTile");
    m_lastTile = tag.getInt8("inTile") & 255;
    m_shakeTime = tag.getInt8("shake") & 255;
    m_bInGround = tag.getBoolean("inGround");
    m_bIsPlayerOwned = tag.getBoolean("player");
}

Entity::AuxValue Snowball::getAuxValue() const
{
    return m_owner ? m_owner->m_EntityID : 0;
}

void Snowball::setAuxValue(Entity::AuxValue value)
{
    Entity* pOwner = m_pLevel->getEntity(value);
    if (pOwner && pOwner->isMob())
        m_owner = (Mob*)pOwner;
}
