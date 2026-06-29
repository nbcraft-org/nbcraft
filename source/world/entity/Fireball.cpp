#include "Fireball.hpp"
#include "Mob.hpp"
#include "world/level/Level.hpp"
#include "nbt/CompoundTag.hpp"

const unsigned int Fireball::ARROW_BASE_DAMAGE = 0;

void Fireball::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::largeFireball;
    m_renderType = RENDER_FIREBALL;
    setSize(1.0f, 1.0f);

    m_tilePos = TilePos(-1, -1, -1);
    m_lastTile = 0;
    m_lastTileData = 0;
    m_bInGround = false;
    m_bIsPlayerOwned = false;
    m_life = 0;
    m_flightTime = 0;
    m_shakeTime = 0;
    m_owner = nullptr;
    m_powerVel = Vec3::ZERO;
}

Fireball::Fireball(Level* pLevel) : Entity(pLevel)
{
    _init();
}

Fireball::Fireball(Level* pLevel, Mob* pMob, Vec3 pos) : Entity(pLevel)
{
    _init();

    m_owner = pMob;
    m_bIsPlayerOwned = m_owner->isPlayer();
    moveTo(Vec3(pMob->m_pos.x, pMob->m_pos.y, pMob->m_pos.z), pMob->m_rot);

    setPos(m_pos);

    m_vel = Vec3::ZERO;
    pos.x += sharedRandom.nextGaussian() * 0.4f;
    pos.y += sharedRandom.nextGaussian() * 0.4f;
    pos.z += sharedRandom.nextGaussian() * 0.4f;

    float len = pos.length();
    m_powerVel.x = pos.x / len * 0.1f;
    m_powerVel.y = pos.y / len * 0.1f;
    m_powerVel.z = pos.z / len * 0.1f;
}

bool Fireball::shouldRenderAtSqrDistance(float distSqr) const
{
    float var3 = (((m_hitbox.max.z - m_hitbox.min.z) + (m_hitbox.max.x - m_hitbox.min.x) + (m_hitbox.max.y - m_hitbox.min.y)) / 3.0f) * 4.0f; // this.bb.getSize() * 4.0D
    var3 *= 64.0f;
    return distSqr < var3 * var3;
}

void Fireball::tick()
{
    Entity::tick();
    m_fireTicks = 10;

    //_lerpMotion2(m_vel);

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
    else
    {
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
        if (ent->isPickable() && (ent != m_owner || m_flightTime >= 25))
        {
            AABB aabb = ent->m_hitbox;
            aabb.grow(var10);
            // these Vec3's are copied in the TilePos::clip fn, so no need to create them over and over like in b1.2
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
        if (hit_result.m_pEnt != nullptr && hit_result.m_pEnt->hurt(m_owner, ARROW_BASE_DAMAGE))
        {
        }

        m_pLevel->explode(nullptr, m_pos, 1.0f, true);
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
    float dampening = 0.95f;
    if (isInWater())
    {
        for (int var19 = 0; var19 < 4; ++var19)
        {
            constexpr float var20 = 0.25f;
            m_pLevel->addParticle("bubble", m_pos - m_vel * var20, m_pos * 1); // passed as reference so *1; although addParticle doesn't exist yet
        }

        dampening = 0.8f;
    }

    m_vel += m_powerVel;
    m_vel *= dampening;
    m_pLevel->addParticle("smoke", Vec3(m_pos.x, m_pos.y + 0.5f, m_pos.z));
    setPos(m_pos);
}

void Fireball::addAdditionalSaveData(CompoundTag& tag) const
{
    tag.putInt16("xTile", m_tilePos.x);
    tag.putInt16("yTile", m_tilePos.y);
    tag.putInt16("zTile", m_tilePos.z);
    tag.putInt8("inTile", m_lastTile);
    tag.putInt8("inData", m_lastTileData);
    tag.putInt8("shake", m_shakeTime);
    tag.putBoolean("inGround", m_bInGround);
    tag.putBoolean("player", m_bIsPlayerOwned);
}

void Fireball::readAdditionalSaveData(const CompoundTag& tag)
{
    m_tilePos.x = tag.getInt16("xTile");
    m_tilePos.y = tag.getInt16("yTile");
    m_tilePos.z = tag.getInt16("zTile");
    m_lastTile = tag.getInt8("inTile") & 255;
    m_lastTileData = tag.getInt8("inData") & 255;
    m_shakeTime = tag.getInt8("shake") & 255;
    m_bInGround = tag.getBoolean("inGround");
    m_bIsPlayerOwned = tag.getBoolean("player");

    // re-init driving force from velocity
    m_powerVel = m_vel;
}

bool Fireball::hurt(Entity* pCulprit, int damage)
{
    markHurt();
    if (pCulprit)
    {
        Vec3 var3 = pCulprit->getLookAngle();
        if (var3 != Vec3::ZERO)
        {
            m_vel = var3;
            m_powerVel = m_vel * 0.1f;
        }

        return true;
    }
    else
    {
        return false;
    }
}
