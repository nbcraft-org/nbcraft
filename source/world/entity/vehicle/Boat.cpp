#include "Boat.hpp"
#include "world/entity/Player.hpp"
#include "world/level/Level.hpp"

void Boat::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::boat;
    m_renderType = RENDER_BOAT;
    m_lPos = Vec3::ZERO;
    m_lRot = Rot2::ZERO;
    m_lPosD = Vec3::ZERO;
    m_lSteps = 0;
    m_damage = 0;
    m_hurtTime = 0;
    m_hurtDir = 1;
    m_bBlocksBuilding = true;
    setSize(1.5f, 0.6f);
    m_heightOffset = m_bbHeight / 2.0f;
    m_bMakeStepSound = false;
}

Boat::Boat(Level* level) : Entity(level)
{
    _init();
}

Boat::Boat(Level* level, const Vec3& pos) : Entity(level)
{
    _init();
	setPos(Vec3(pos.x, pos.y + m_heightOffset, pos.z));
	m_vel = Vec3::ZERO;
	m_oPos = Vec3::ZERO;
}

const AABB* Boat::getCollideBox() const
{
    return &m_hitbox;
}

AABB* Boat::getCollideAgainstBox(Entity* ent) const
{
    return &ent->m_hitbox;
}

bool Boat::interact(Player* player)
{
    if (getRider() && getRider()->isPlayer() && getRider() != player)
        return true;

    if (!m_pLevel->m_bIsClientSide)
        player->ride(this);

    return true;
}

void Boat::tick()
{
    if (m_hurtTime > 0)
        --m_hurtTime;

    if (m_damage > 0)
        --m_damage;


    m_oPos = m_pos;
    float var2 = 0.0f;

    for (int var4 = 0; var4 < 5; ++var4)
    {
        float var5 = m_hitbox.min.y + (m_hitbox.max.y - m_hitbox.min.y) * (var4 + 0) / 5 - 0.125;
        //float var7 = m_hitbox.min.y + (m_hitbox.max.y - m_hitbox.min.y) * (var4 + 1) / 5 - 0.125;
        AABB var9 = AABB(m_hitbox.min.x, var5, m_hitbox.min.z, m_hitbox.max.x, var5, m_hitbox.max.z);
        if (m_pLevel->containsLiquid(var9, Material::water))
        {
            var2 += 1.0f / 5;
        }
    }

    float var6;
    float var8;
    float var10;
    float var23;
    if (m_pLevel->m_bIsClientSide)
    {
        if (m_lSteps > 0)
        {
            Vec3 newPos = m_pos + (m_lPos - m_pos) / m_lSteps;

            for (var10 = m_lRot.yaw - m_rot.yaw; var10 < -180.0f; var10 += 360.0f) {
            }

            while (var10 >= 180.0f)
                var10 -= 360.0f;

            m_rot.yaw = m_rot.yaw + var10 / m_lSteps;
            m_rot.pitch = m_rot.pitch + (m_lRot.pitch - m_rot.pitch) / m_lSteps;
            --m_lSteps;
            setPos(newPos);
            setRot(m_rot);
        }
        else
        {
            setPos(m_pos + m_vel);
            if (m_bOnGround) {
                m_vel.x *= 0.5;
                m_vel.y *= 0.5;
                m_vel.z *= 0.5;
            }

            m_vel.x *= 0.99;
            m_vel.y *= 0.95;
            m_vel.z *= 0.99;
        }

    }
    else
    {
        var23 = var2 * 2.0f - 1.0f;
        m_vel.y += 0.04 * var23;
        if (getRider())
        {
            m_vel.x += getRider()->m_vel.x * 0.2;
            m_vel.z += getRider()->m_vel.z * 0.2;
        }

        var6 = 0.4;
        if (m_vel.x < -var6)
            m_vel.x = -var6;

        if (m_vel.x > var6)
            m_vel.x = var6;

        if (m_vel.z < -var6)
            m_vel.z = -var6;

        if (m_vel.z > var6)
            m_vel.z = var6;

        if (m_bOnGround)
        {
            m_vel *= 0.5;
        }

        move(m_vel);
        var8 = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
        float var12;
        if (var8 > 0.15) {
            var10 = Mth::cos(m_rot.yaw * M_PI / 180.0f);
            var12 = Mth::sin(m_rot.pitch * M_PI / 180.0f);

            for (int var14 = 0; var14 < 1.0f + var8 * 60.0f; ++var14) {
                float var15 = (m_random.nextFloat() * 2.0f - 1.0f);
                float var17 = (m_random.nextInt(2) * 2 - 1) * 0.7;
                float var19;
                float var21;
                if (m_random.nextInt(1))
                {
                    var19 = m_pos.x - var10 * var15 * 0.8 + var12 * var17;
                    var21 = m_pos.z - var12 * var15 * 0.8 - var10 * var17;
                    m_pLevel->addParticle("splash", Vec3(var19, m_pos.y - 0.125, var21), m_vel);
                }
                else {
                    var19 = m_pos.x + var10 + var12 * var15 * 0.7;
                    var21 = m_pos.z + var12 - var10 * var15 * 0.7;
                    m_pLevel->addParticle("splash", Vec3(var19, m_pos.y - 0.125, var21), m_vel);
                }
            }
        }

        if (m_bHorizontalCollision && var8 > 0.15)
        {
            if (!m_pLevel->m_bIsClientSide)
            {
                remove();

                int var24;
                for (var24 = 0; var24 < 3; ++var24)
                    spawnAtLocation(Tile::wood->m_ID, 1, 0.0f);

                for (var24 = 0; var24 < 2; ++var24)
                    spawnAtLocation(Item::stick->m_itemID, 1, 0.0f);
            }
        }
        else
        {
            m_vel.x *= 0.99f;
            m_vel.y *= 0.95f;
            m_vel.z *= 0.99f;
        }

        m_rot.pitch = 0.0f;
        var10 = m_rot.yaw;
        var12 = m_oPos.x - m_pos.x;
        float var25 = m_oPos.z - m_pos.z;
        if (var12 * var12 + var25 * var25 > 0.001f)
            var10 = Mth::atan2(var25, var12) * 180.0f / M_PI;

        float var16;
        for (var16 = var10 - m_rot.yaw; var16 >= 180.0f; var16 -= 360.0f) {
        }

        while (var16 < -180.0f)
            var16 += 360.0f;

        if (var16 > 20.0f)
            var16 = 20.0f;

        if (var16 < -20.0f)
            var16 = -20.0f;

        m_rot.yaw = m_rot.yaw + var16;
        setRot(m_rot);
        AABB hit = m_hitbox;
        hit.grow(0.2, 0.0f, 0.2);
        EntityVector var18 = m_pLevel->getEntities(this, hit);
        if (var18.size() > 0)
        {
            for (EntityVector::iterator it = var18.begin();  it != var18.end(); ++it)
            {
                Entity* entity = *it;
                if (entity != getRider() && entity->isPushable() && entity->getDescriptor().getEntityType() == EntityType::BOAT)
                    entity->push(this);
            }
        }

        if (getRider() && getRider()->m_bRemoved)
            setRider(nullptr);
    }
}

float Boat::getRideHeight() const
{
	return -0.3f;
}

void Boat::positionRider() 
{
    if (getRider())
    {
        getRider()->setPos(Vec3(m_pos.x + Mth::cos(m_rot.yaw * M_PI / 180.0f) * 0.4, m_pos.y + getRideHeight() + getRider()->getRidingHeight(), m_pos.z + Mth::sin(m_rot.yaw * M_PI / 180.0f) * 0.4));
    }
}

bool Boat::hurt(Entity*, int mul)
{
    m_hurtDir = -m_hurtDir;
    m_hurtTime = 10;
    markHurt();
    if (m_damage < 40)
        m_damage += mul * 10;

    if (!m_pLevel->m_bIsClientSide && !m_bRemoved) 
    {
        if (m_damage > 40) 
        {
            for (int i = 0; i < 3; ++i) {
                spawnAtLocation(Tile::wood->m_ID, 1, 0.0f);
            }

            for (int i = 0; i < 2; ++i) {
                spawnAtLocation(Item::stick->m_itemID, 1, 0.0f);
            }

            remove();
        }
        return true;
    }
    else
        return true;
}

void Boat::animateHurt()
{
    m_hurtDir = -m_hurtDir;
    m_hurtTime = 10;
    if (m_damage < 40)
        m_damage += m_damage * 10;
}

void Boat::lerpTo(const Vec3& pos, const Rot2& rot, int steps)
{
    m_lPos = pos;
    m_lRot = rot;
    m_lSteps = steps + 4;
    m_vel = m_lPosD;
}

void Boat::lerpMotion(const Vec3& motion) 
{
    m_lPosD = m_vel = motion;
}
