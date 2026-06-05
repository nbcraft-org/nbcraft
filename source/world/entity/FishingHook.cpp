#include "FishingHook.hpp"
#include "Mob.hpp"
#include "world/level/Level.hpp"
#include "nbt/CompoundTag.hpp"

const unsigned int FishingHook::ARROW_BASE_DAMAGE = 0;

void FishingHook::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::fishingHook;
    m_renderType = RENDER_FISHING_HOOK;
    setSize(0.25f, 0.25f);

    m_tilePos = TilePos(-1, -1, -1);
    m_lastTile = 0;
    m_lastTileData = 0;
    m_bInGround = false;
    m_bIsPlayerOwned = false;
    m_life = 0;
    m_flightTime = 0;
    m_shakeTime = 0;
    m_owner = nullptr;
    m_nibble = 0;
    m_hookedIn = nullptr;
}

FishingHook::FishingHook(Level* pLevel) : Entity(pLevel)
{
    _init();
}

FishingHook::FishingHook(Level* pLevel, const Vec3& pos) : Entity(pLevel)
{
    _init();

    setPos(pos);
}

FishingHook::FishingHook(Level* pLevel, Player* pMob) : Entity(pLevel)
{
    _init();

    m_owner = pMob;
    m_owner->m_pFishing = this;
    m_bIsPlayerOwned = m_owner->isPlayer();
    moveTo(Vec3(pMob->m_pos.x, pMob->m_pos.y + 1.62f - pMob->m_heightOffset, pMob->m_pos.z), Vec2(pMob->m_rot.y, pMob->m_rot.x));

    m_pos.x -= Mth::cos(m_rot.y / 180.0f * M_PI) * 0.16f;
    m_pos.y -= 0.1f;
    m_pos.z -= Mth::sin(m_rot.y / 180.0f * M_PI) * 0.16f;
    setPos(m_pos);
    m_heightOffset = 0.0f;
    float var3 = 0.4f;
    m_vel.x = -Mth::sin(m_rot.y / 180.0f * M_PI) * Mth::cos(m_rot.x / 180.0f * M_PI) * var3;
    m_vel.z = Mth::cos(m_rot.y / 180.0f * M_PI) * Mth::cos(m_rot.x / 180.0f * M_PI) * var3;
    m_vel.y = -Mth::sin(m_rot.x / 180.0f * M_PI) * var3;
    shoot(m_vel, 1.5f, 1.0f);
}

bool FishingHook::shouldRenderAtSqrDistance(float distSqr) const
{
    float var3 = (((m_hitbox.max.z - m_hitbox.min.z) + (m_hitbox.max.x - m_hitbox.min.x) + (m_hitbox.max.y - m_hitbox.min.y)) / 3.0f) * 4.0f; // this.bb.getSize() * 4.0D
    var3 *= 64.0f;
    return distSqr < var3 * var3;
}

void FishingHook::shoot(Vec3 vel, float speed, float r)
{
    float len = vel.length();
    vel /= len;
    vel.x += sharedRandom.nextGaussian() * 0.0075f * r;
    vel.y += sharedRandom.nextGaussian() * 0.0075f * r;
    vel.z += sharedRandom.nextGaussian() * 0.0075f * r;
    vel *= speed;

    m_vel = vel;
    _lerpMotion(vel);

    m_life = 0;
}

void FishingHook::lerpTo(const Vec3& pos, const Vec2& rot, int steps) {
    m_lPos = pos;
    m_lRot = rot;
    m_lSteps = steps;
    m_vel = m_lVel;
}

void FishingHook::_lerpMotion(const Vec3& vel)
{
    float len = vel.length();
    m_oRot.y = m_rot.y = Mth::atan2(vel.x, vel.z) * 180.0f / M_PI;
    m_oRot.x = m_rot.x = Mth::atan2(vel.y, len) * 180.0f / M_PI;
}

void FishingHook::_lerpMotion2(const Vec3& vel)
{
    if (m_oRot.x == 0.0f && m_oRot.y == 0.0f)
    {
        return _lerpMotion(vel);
    }
}

void FishingHook::lerpMotion(const Vec3& vel)
{
    m_lVel = m_vel = vel;

    _lerpMotion2(vel);
}

void FishingHook::tick()
{
    Entity::tick();

    //_lerpMotion2(m_vel);

    if (m_lSteps > 0)
    {
        float var21 = m_pos.x + (m_lPos.x - m_pos.x) / float(m_lSteps);
        float var22 = m_pos.y + (m_lPos.y - m_pos.y) / float(m_lSteps);
        float var23 = m_pos.z + (m_lPos.z - m_pos.z) / float(m_lSteps);

        float var7;
        for (var7 = m_lRot.y - m_rot.y; var7 < -180.0f; var7 += 360.0f)
        {
        }

        while (var7 >= 180.0f)
        {
            var7 -= 360.0f;
        }

        m_rot.y = float(m_rot.y + var7 / float(m_lSteps));
        m_rot.x = float(m_rot.x + (m_lRot.x - m_rot.x) / float(m_lSteps));
        --m_lSteps;
        setPos(Vec3(var21, var22, var23));
        setRot(m_rot);
    }
    else
    {
        if (!m_pLevel->m_bIsClientSide)
        {
            ItemStack var1 = m_owner->getSelectedItem();
            if (m_owner->m_bRemoved || !m_owner->isAlive() || !var1 || var1.getItem() != Item::fishingRod || distanceToSqr(m_owner) > 1024.0f)
            {
                remove();
                m_owner->m_pFishing = nullptr;
                return;
            }

            if (m_hookedIn)
            {
                if (!m_hookedIn->m_bRemoved)
                {
                    m_pos.x = m_hookedIn->m_pos.x;
                    m_pos.y = m_hookedIn->m_hitbox.min.y + float(m_hookedIn->m_bbHeight) * 0.8f;
                    m_pos.z = m_hookedIn->m_pos.z;
                    return;
                }

                m_hookedIn = nullptr;
            }
        }

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
        float distance; // var13
        constexpr float var10 = 0.3f;
        for (EntityVector::iterator it = entities.begin(); it != entities.end(); it++)
        {
            Entity* ent = *it;
            if (ent->isPickable() && (ent != m_owner || m_flightTime >= 5))
            {
                AABB aabb = ent->m_hitbox;
                aabb.grow(var10);
                // these Vec3's are copied in the TilePos::clip fn, so no need to create them over and over like in b1.2
                HitResult hit = aabb.clip(m_pos, future_pos);
                if (hit.isHit())
                {
                    distance = m_pos.distanceTo(hit.m_hitPos);
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
                m_hookedIn = hit_result.m_pEnt;
            }
            else
            {
                m_bInGround = true;
            }
        }

        if (!m_bInGround)
        {
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
            float dampening = 0.92f; // var25
            if (m_bOnGround || m_bHorizontalCollision)
            {
                dampening = 0.5f;
            }

            int var26 = 5;
            float var27 = 0.0f;

            for (int var28 = 0; var28 < var26; ++var28)
            {
                float var14 = m_hitbox.min.y + (m_hitbox.max.y - m_hitbox.min.y) * float(var28 + 0) / float(var26) - 0.125f + 0.125f;
                float var16 = m_hitbox.min.y + (m_hitbox.max.y - m_hitbox.min.y) * float(var28 + 1) / float(var26) - 0.125f + 0.125f;
                AABB var18 = AABB(m_hitbox.min.x, var14, m_hitbox.min.z, m_hitbox.max.x, var16, m_hitbox.max.z);
                if (m_pLevel->containsLiquid(var18, Material::water))
                {
                    var27 += 1.0f / float(var26);
                }
            }

            if (var27 > 0.0f)
            {
                if (m_nibble > 0)
                {
                    --m_nibble;
                }
                else if (sharedRandom.nextInt(500) == 0)
                {
                    m_nibble = sharedRandom.nextInt(30) + 10;
                    m_vel.y -= 0.2f;
                    m_pLevel->playSound(this, "random.splash", 0.25f, 1.0f + (sharedRandom.nextFloat() - sharedRandom.nextFloat()) * 0.4f);
                    float var29 = float(Mth::floor(m_hitbox.min.y));

                    float var15;
                    int var30;
                    float var31;
                    for (var30 = 0; float(var30) < 1.0f + m_bbWidth * 20.0f; ++var30)
                    {
                        var15 = (sharedRandom.nextFloat() * 2.0f - 1.0f) * m_bbWidth;
                        var31 = (sharedRandom.nextFloat() * 2.0f - 1.0f) * m_bbWidth;
                        m_pLevel->addParticle("bubble", Vec3(m_pos.x + var15, var29 + 1.0f, m_pos.z + var31), Vec3(m_vel.x, m_vel.y - (sharedRandom.nextFloat() * 0.2f), m_vel.z));
                    }

                    for (var30 = 0; float(var30) < 1.0f + m_bbWidth * 20.0f; ++var30)
                    {
                        var15 = (sharedRandom.nextFloat() * 2.0f - 1.0f) * m_bbWidth;
                        var31 = (sharedRandom.nextFloat() * 2.0f - 1.0f) * m_bbWidth;
                        m_pLevel->addParticle("splash", Vec3(m_pos.x + var15, var29 + 1.0f, m_pos.z + var31), m_vel);
                    }
                }
            }

            if (m_nibble > 0)
            {
                m_vel.y -= (sharedRandom.nextFloat() * sharedRandom.nextFloat() * sharedRandom.nextFloat()) * 0.2f;
            }

            distance = var27 * 2.0f - 1.0f;
            m_vel.y += 0.04f * distance;
            if (var27 > 0.0f)
            {
                dampening = dampening * 0.9f;
                m_vel.y *= 0.8f;
            }

            m_vel *= dampening;
            //m_vel.y -= 0.03f;
            setPos(m_pos);
        }
    }
}

void FishingHook::addAdditionalSaveData(CompoundTag& tag) const
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

void FishingHook::readAdditionalSaveData(const CompoundTag& tag)
{
    m_tilePos.x = tag.getInt16("xTile");
    m_tilePos.y = tag.getInt16("yTile");
    m_tilePos.z = tag.getInt16("zTile");
    m_lastTile = tag.getInt8("inTile") & 255;
    m_lastTileData = tag.getInt8("inData") & 255;
    m_shakeTime = tag.getInt8("shake") & 255;
    m_bInGround = tag.getBoolean("inGround");
    m_bIsPlayerOwned = tag.getBoolean("player");
}

int FishingHook::retrieve()
{
    int dmg = 0; // var1
    if (m_hookedIn)
    {
        float var2 = m_owner->m_pos.x - m_pos.x;
        float var4 = m_owner->m_pos.y - m_pos.y;
        float var6 = m_owner->m_pos.z - m_pos.z;
        float var8 = Mth::sqrt(var2 * var2 + var4 * var4 + var6 * var6);
        float var10 = 0.1f;
        m_hookedIn->m_vel.x += var2 * var10;
        m_hookedIn->m_vel.y += var4 * var10 + Mth::sqrt(var8) * 0.08f;
        m_hookedIn->m_vel.z += var6 * var10;
        dmg = 3;
    }
    else if (m_nibble > 0)
    {
        ItemEntity* var13 = new ItemEntity(m_pLevel, m_pos, ItemStack(Item::fish_raw));
        float var3 = m_owner->m_pos.x - m_pos.x;
        float var5 = m_owner->m_pos.y - m_pos.y;
        float var7 = m_owner->m_pos.z - m_pos.z;
        float var9 = Mth::sqrt(var3 * var3 + var5 * var5 + var7 * var7);
        float var11 = 0.1f;
        var13->m_vel.x = var3 * var11;
        var13->m_vel.y = var5 * var11 + Mth::sqrt(var9) * 0.08f;
        var13->m_vel.z = var7 * var11;
        m_pLevel->addEntity(var13);
        dmg = 1;
    }

    if (m_bInGround)
    {
        dmg = 2;
    }

    remove();
    m_owner->m_pFishing = nullptr;
    return dmg;
}
