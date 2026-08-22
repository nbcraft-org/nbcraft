#include "Minecart.hpp"
#include "world/entity/Player.hpp"
#include "world/level/Level.hpp"
#include "world/tile/RailTile.hpp"

const TilePos Minecart::EXITS[10][2] = {
    {TilePos(0, 0, -1), TilePos(0, 0, 1)},
    {TilePos(-1, 0, 0), TilePos(1, 0, 0)},
    {TilePos(-1, -1, 0), TilePos(1, 0, 0)},
    {TilePos(-1, 0, 0), TilePos(1, -1, 0)},
    {TilePos(0, 0, -1), TilePos(0, -1, 1)},
    {TilePos(0, -1, -1), TilePos(0, 0, 1)},
    {TilePos(0, 0, 1), TilePos(1, 0, 0)},
    {TilePos(0, 0, 1), TilePos(-1, 0, 0)},
    {TilePos(0, 0, -1), TilePos(-1, 0, 0)},
    {TilePos(0, 0, -1), TilePos(1, 0, 0)}
};

void Minecart::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::minecartRideable;
    m_renderType = RENDER_MINECART;
    m_lPos = Vec3::ZERO;
    m_lRot = Rot2::ZERO;
    m_lPosD = Vec3::ZERO;
    m_lSteps = 0;
    m_pushX = 0;
    m_pushZ = 0;
    m_fuel = 0;
    m_type = TYPE_DEFAULT;
    m_damage = 0;
    m_hurtTime = 0;
    m_hurtDir = 1;
    m_bFlipped = false;
    m_bBlocksBuilding = true;
    setSize(0.98f, 0.7f);
    m_heightOffset = m_bbHeight / 2.0f;
    m_bMakeStepSound = false;
}

Minecart::Minecart(TileSource& source)
    : Entity(source)
    , SimpleContainer(27, "Minecart")
{
    _init();
}

Minecart::Minecart(TileSource& source, Type type, const Vec3& pos)
    : Entity(source)
    , SimpleContainer(27, "Minecart")
{
    _init();
	setPos(Vec3(pos.x, pos.y + m_heightOffset, pos.z));
	m_vel = Vec3::ZERO;
	m_oPos = Vec3::ZERO;
	m_type = type;
}

const AABB* Minecart::getCollideBox() const
{
    return nullptr;
}

AABB* Minecart::getCollideAgainstBox(Entity* ent) const
{
    return &ent->m_hitbox;
}

bool Minecart::interact(Player* player)
{
    switch (m_type)
    {
    case TYPE_DEFAULT:
    {
        if (getRider() && getRider()->isPlayer() && getRider() != player)
            return true;

        if (!m_pLevel->m_bIsClientSide)
            player->ride(this);

        break;
    }
    case TYPE_CHEST:
    {
        if (!m_pLevel->m_bIsClientSide)
            player->openContainer(this);

        break;
    }
    case TYPE_FURNACE:
    {
        ItemStack& heldItem = player->m_pInventory->getSelected();
        if (heldItem && heldItem.getId() == Item::coal->m_itemID)
        {
            if (--heldItem.m_count == 0)
                player->m_pInventory->setSelectedItem(ItemStack::EMPTY);

            m_fuel += 1200;
        }

        m_pushX = m_pos.x - player->m_pos.x;
        m_pushZ = m_pos.z - player->m_pos.z;

        break;
    }
    }

    return true;
}

inline static float normalizeAngle(float angle)
{
	while (angle >= 180.0f)
		angle -= 360.0f;
	while (angle < -180.0f)
		angle += 360.0f;
	return angle;
}

void Minecart::_adjustVelocity(const TilePos& tp, bool& canPush)
{
    static constexpr float C_MAX_VEL = 0.4f;
    static constexpr float C_GRAVITY = 0.0078125f;

    m_vel.y -= 0.04f;

    TileID tile = m_pLevel->getTile(tp);
    if (!RailTile::isRail(tile))
    {
        m_vel.x = Mth::clamp(m_vel.x, -C_MAX_VEL, C_MAX_VEL);
        m_vel.z = Mth::clamp(m_vel.z, -C_MAX_VEL, C_MAX_VEL);

        if (m_bOnGround)
            m_vel *= 0.5f;

        move(m_vel);

        if (!m_bOnGround)
            m_vel *= 0.95f;

        return;
    }

    RailTile* rail = (RailTile*)Tile::tiles[tile];
    Vec3* oRailPos = getOnRailPos(m_pos);
    TileData data = m_pLevel->getData(tp);
    m_pos.y = tp.y;

    bool hasPower = rail->getPowered(data);
	data = rail->getFaceData(data);
    
    if (data >= RailTile::WEST_EAST_ABOVE && data <= RailTile::NORTH_SOUTH_ABOVE)
        m_pos.y = tp.y + 1;

    if (data == RailTile::WEST_EAST_ABOVE)
        m_vel.x -= C_GRAVITY;
    else if (data == RailTile::EAST_WEST_ABOVE)
        m_vel.x += C_GRAVITY;
    else if (data == RailTile::SOUTH_NORTH_ABOVE)
        m_vel.z += C_GRAVITY;
    else if (data == RailTile::NORTH_SOUTH_ABOVE)
        m_vel.z -= C_GRAVITY;

    const TilePos* exits = EXITS[data];

    float var12 = exits[1].x - exits[0].x;
    float var14 = exits[1].z - exits[0].z;
    float var16 = Mth::sqrt(var12 * var12 + var14 * var14);
    float var18 = m_vel.x * var12 + m_vel.z * var14;
    if (var18 < 0.0f)
    {
        var12 = -var12;
        var14 = -var14;
    }

    float velDist = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
    m_vel.x = velDist * var12 / var16;
    m_vel.z = velDist * var14 / var16;
    if (RailTile::isPoweredRail(rail) && !hasPower)
    {
        float velDist = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
        if (velDist < 0.03)
            m_vel *= 0;
        else
        {
            m_vel.x *= 0.5f;
            m_vel.y *= 0.0f;
            m_vel.z *= 0.5f;
        }
    }

    float var22 = 0.0f;
    float var24 = tp.x + 0.5f + exits[0].x * 0.5f;
    float var26 = tp.z + 0.5f + exits[0].z * 0.5f;
    float var28 = tp.x + 0.5f + exits[1].x * 0.5f;
    float var30 = tp.z + 0.5f + exits[1].z * 0.5f;
    var12 = var28 - var24;
    var14 = var30 - var26;
    if (var12 == 0.0f)
    {
        m_pos.x = tp.x + 0.5f;
        var22 = m_pos.z - tp.z;
    }
    else if (var14 == 0.0f)
    {
        m_pos.z = tp.z + 0.5f;
        var22 = m_pos.x - tp.x;
    }
    else
    {
        float var32 = m_pos.x - var24;
        float var34 = m_pos.z - var26;
        var22 = (var32 * var12 + var34 * var14) * 2.0;
    }

    m_pos.x = var24 + var12 * var22;
    m_pos.z = var26 + var14 * var22;
    setPos(Vec3(m_pos.x, m_pos.y + m_heightOffset, m_pos.z));
    float velX = m_vel.x;
    float velZ = m_vel.z;
    if (getRider())
    {
        velX *= 0.75f;
        velZ *= 0.75f;
    }

    velX = Mth::clamp(velX, -C_MAX_VEL, C_MAX_VEL);
    velZ = Mth::clamp(velZ, -C_MAX_VEL, C_MAX_VEL);

    move(Vec3(velX, 0.0f, velZ));
    if (exits[0].y != 0 && Mth::floor(m_pos.x) - tp.x == exits[0].x 
        && Mth::floor(m_pos.z) - tp.z == exits[0].z)
    {
        setPos(Vec3(m_pos.x, m_pos.y + exits[0].y, m_pos.z));
    }
    else if (exits[1].y != 0 && Mth::floor(m_pos.x) - tp.x == exits[1].x 
        && Mth::floor(m_pos.z) - tp.z == exits[1].z)
    {
        setPos(Vec3(m_pos.x, m_pos.y + exits[1].y, m_pos.z));
    }

    if (getRider())
    {
        m_vel.x *= 0.997f;
        m_vel.y *= 0.0f;
        m_vel.z *= 0.997f;
    }
    else
    {
        if (m_type == TYPE_FURNACE)
        {
            float pushDist = Mth::sqrt(m_pushX * m_pushX + m_pushZ * m_pushZ);
            if (pushDist > 0.01f)
            {
                canPush = true;
                m_pushX /= pushDist;
                m_pushZ /= pushDist;
                m_vel.x *= 0.8f;
                m_vel.y *= 0.0f;
                m_vel.z *= 0.8f;
                m_vel.x += m_pushX * 0.04f;
                m_vel.z += m_pushZ * 0.04f;
            }
            else
            {
                m_vel.x *= 0.9f;
                m_vel.y *= 0.0f;
                m_vel.z *= 0.9f;
            }
        }

        m_vel.x *= 0.96f;
        m_vel.y *= 0.0f;
        m_vel.z *= 0.96f;
    }

    if (hasPower)
    {
        float velDist = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
        if (velDist > 0.01f)
        {
            constexpr float speed = 0.06f;
            m_vel.x += m_vel.x / velDist * speed;
            m_vel.z += m_vel.z / velDist * speed;
        }
        else if (data == RailTile::WEST_EAST)
        {
            if (m_pLevel->isSolidBlockingTile(tp.west()))
                m_vel.x = 0.02f;
            else if (m_pLevel->isSolidBlockingTile(tp.east()))
                m_vel.x = -0.02f;
        }
        else if (data == RailTile::NORTH_SOUTH)
        {
            if (m_pLevel->isSolidBlockingTile(tp.north()))
                m_vel.z = 0.02f;
            else if (m_pLevel->isSolidBlockingTile(tp.south()))
                m_vel.z = -0.02f;
        }
    }

    Vec3* railPos = getOnRailPos(m_pos);
    if (railPos && oRailPos)
    {
        float yOffset = (oRailPos->y - railPos->y) * 0.05f;
        float velDist = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
        if (velDist > 0.0f)
        {
            m_vel.x = m_vel.x / velDist * (velDist + yOffset);
            m_vel.z = m_vel.z / velDist * (velDist + yOffset);
        }

        setPos(Vec3(m_pos.x, railPos->y, m_pos.z));
    }
    SAFE_DELETE(oRailPos);
    SAFE_DELETE(railPos);

    int xFloored = Mth::floor(m_pos.x);
    int zFloored = Mth::floor(m_pos.z);
    float velSq = m_vel.x * m_vel.x + m_vel.z * m_vel.z;
    if (xFloored != tp.x || zFloored != tp.z)
    {
        float velSqrt = Mth::sqrt(velSq);
        m_vel.x = velSqrt * (xFloored - tp.x);
        m_vel.z = velSqrt * (zFloored - tp.z);
    }

    if (m_type == TYPE_FURNACE)
    {
        float pushSqrt = Mth::sqrt(m_pushX * m_pushX + m_pushZ * m_pushZ);
        if (pushSqrt > 0.01f && velSq > 0.001f)
        {
            m_pushX /= pushSqrt;
            m_pushZ /= pushSqrt;
            if (m_pushX * m_vel.x + m_pushZ * m_vel.z < 0.0f)
            {
                m_pushX = 0.0f;
                m_pushZ = 0.0f;
            }
            else
            {
                m_pushX = m_vel.x;
                m_pushZ = m_vel.z;
            }
        }
    }
}

void Minecart::_adjustRotation()
{
    m_rot.pitch = 0.0f;
    float oXDiff = m_oPos.x - m_pos.x;
    float oZDiff = m_oPos.z - m_pos.z;
    if (oXDiff * oXDiff + oZDiff * oZDiff > 0.001f)
    {
        m_rot.yaw = Mth::atan2(oZDiff, oXDiff) * 180.0f / M_PI;

        if (m_bFlipped)
            m_rot.yaw += 180.0f;
    }

    float yaw = normalizeAngle(m_rot.yaw - m_oRot.yaw);
    if (yaw < -170.0f || yaw >= 170.0f)
    {
        m_rot.yaw += 180.0f;
        m_bFlipped = !m_bFlipped;
    }

    setRot(m_rot);
}

void Minecart::tick()
{
    if (m_hurtTime > 0) 
        --m_hurtTime;

    if (m_damage > 0)
        --m_damage;

    if (m_pLevel->m_bIsClientSide && m_lSteps > 0) 
    {
        m_rot.yaw = m_rot.yaw + normalizeAngle(m_lRot.yaw - m_rot.yaw) / m_lSteps;
        m_rot.pitch = m_rot.pitch + (m_lRot.pitch - m_rot.pitch) / m_lSteps;

        setPos(m_pos + (m_lPos - m_pos) / m_lSteps);
        setRot(m_rot);
        --m_lSteps;
        return;
    }

    // Host/singleplayer:
    m_oPos = m_pos;
    TilePos tp(m_pos);
    if (RailTile::isRail(m_pLevel->getTile(tp.below())))
        --tp.y;
     
    bool canPush = false;
    _adjustVelocity(tp, canPush);
    _adjustRotation();

    AABB hit = m_hitbox;
    hit.grow(0.2f, 0.0f, 0.2f);
    Entity::Vector nearbyEntities = m_pLevel->getEntities(this, hit);
    if (nearbyEntities.size() > 0) 
    {
        for (Entity::Vector::iterator it = nearbyEntities.begin(); it != nearbyEntities.end(); ++it)
        {
            Entity* entity = *it;
            if (entity != getRider() && entity->isPushable() && entity->getDescriptor().hasCategory(EntityCategories::MINECART))
                entity->push(this);
        }
    }

    if (getRider() && getRider()->m_bRemoved)
        setRider(nullptr);

    if (canPush && sharedRandom.nextInt(4) == 0) 
    {
        --m_fuel;
        if (m_fuel < 0)
            m_pushX = m_pushZ = 0.0f;

        m_pLevel->addParticle("largesmoke", Vec3(m_pos.x, m_pos.y + 0.8f, m_pos.z));
    }
}

float Minecart::getRideHeight() const
{
	return -0.3f;
}

bool Minecart::hurt(Entity*, int mul)
{
    if (m_pLevel->m_bIsClientSide || m_bRemoved)
        return false;

    m_hurtDir = -m_hurtDir;
    m_hurtTime = 10;
    markHurt();
    m_damage += mul * 10;
    if (m_damage > 40)
    {
        spawnAtLocation(Item::minecart->m_itemID, 1, 0.0f);
        if (m_type != TYPE_DEFAULT)
            spawnAtLocation(m_type == TYPE_CHEST ? Tile::chest->m_ID : Tile::furnace->m_ID, 1, 0.0f);

        remove();
    }

    return true;
}

void Minecart::animateHurt()
{
    m_hurtDir = -m_hurtDir;
    m_hurtTime = 10;
    m_damage += m_damage * 10;
}

void Minecart::remove()
{
    for (int i = 0; i < getContainerSize(); ++i)
    {
        ItemStack& stack = getItem(i);
        if (!stack.isEmpty())
        {
            float randX = sharedRandom.nextFloat() * 0.8f + 0.1f;
            float randY = sharedRandom.nextFloat() * 0.8f + 0.1f;
            float randZ = sharedRandom.nextFloat() * 0.8f + 0.1f;

            while (stack.m_count > 0)
            {
                int itemCount = sharedRandom.nextInt(21) + 10;
                if (itemCount > stack.m_count) {
                    itemCount = stack.m_count;
                }

                stack.m_count -= itemCount;
                ItemEntity* ent = new ItemEntity(*m_pLevel, Vec3(m_pos.x + randX, m_pos.y + randY, m_pos.z + randZ), ItemStack(stack.getId(), itemCount, stack.getAuxValue()));
                float var8 = 0.05f;
                ent->m_vel.x = sharedRandom.nextGaussian() * var8;
                ent->m_vel.y = sharedRandom.nextGaussian() * var8 + 0.2f;
                ent->m_vel.z = sharedRandom.nextGaussian() * var8;
                m_pLevel->addEntity(ent);
            }
        }
    }

    Entity::remove();
}

Vec3* Minecart::getPosOffs(const Vec3& pos, float var7) const
{
    TilePos tp(pos);
    if (RailTile::isRail(m_pLevel->getTile(tp.below())))
        --tp.y;

    TileID tile = m_pLevel->getTile(tp);
    if (!RailTile::isRail(tile))
        return nullptr;

    Vec3 newPos = pos;
    TileData data = ((RailTile*)Tile::tiles[tile])->getFaceData(m_pLevel->getData(tp));
    newPos.y = tp.y;

    if (data >= RailTile::WEST_EAST_ABOVE && data <= RailTile::NORTH_SOUTH_ABOVE)
        newPos.y = tp.y + 1;

    const TilePos* exits = EXITS[data];

    float var14 = (exits[1].x - exits[0].x);
    float var16 = (exits[1].z - exits[0].z);
    float var18 = Mth::sqrt(var14 * var14 + var16 * var16);
    var14 /= var18;
    var16 /= var18;
    newPos.x += var14 * var7;
    newPos.z += var16 * var7;
    if (exits[0].y != 0 && Mth::floor(pos.x) - tp.x == exits[0].x && Mth::floor(newPos.z) - tp.z == exits[0].z)
        newPos.y += exits[0].y;
    else if (exits[1].y != 0 && Mth::floor(pos.x) - tp.x == exits[1].x && Mth::floor(newPos.z) - tp.z == exits[1].z)
        newPos.y += exits[1].y;

    return getOnRailPos(newPos);
}

void Minecart::addAdditionalSaveData(CompoundTag& tag) const
{
    tag.putInt8("Type", m_type);
    if (m_type == TYPE_FURNACE) 
    {
        tag.putFloat("PushX", m_pushX);
        tag.putFloat("PushZ", m_pushZ);
        tag.putInt16("Fuel", m_fuel);
    } else if (m_type == TYPE_CHEST) 
        SimpleContainer::save(tag);
}

void Minecart::readAdditionalSaveData(const CompoundTag& tag)
{
    m_type = (Type) tag.getInt8("Type");
    if (m_type == TYPE_FURNACE) 
    {
        m_pushX = tag.getDouble("PushX");
        m_pushZ = tag.getDouble("PushZ");
        m_fuel = tag.getInt16("Fuel");
    }
    else if (m_type == TYPE_CHEST)
        SimpleContainer::load(tag);
}

float Minecart::getLootContent()
{
    int count = 0;
    for (int i = 0; i < getContainerSize(); ++i)
    {
        if (getItem(i))
            ++count;
    }
    
    return (float)count / getContainerSize();
}

Vec3* Minecart::getOnRailPos(const Vec3& vec) const
{
    TilePos tp(vec);
    if (RailTile::isRail(m_pLevel->getTile(tp.below())))
        --tp.y;

    TileID tile = m_pLevel->getTile(tp);
    if (RailTile::isRail(tile))
    {
        Vec3* newPos = new Vec3(vec);
        TileData data = ((RailTile*)Tile::tiles[tile])->getFaceData(m_pLevel->getData(tp));
        newPos->y = tp.y;

        if (data >= RailTile::WEST_EAST_ABOVE && data <= RailTile::NORTH_SOUTH_ABOVE)
            newPos->y = (tp.y + 1);

        const TilePos* exits = EXITS[data];

        float var12 = 0.0f;
        float var14 = tp.x + 0.5f + exits[0].x * 0.5f;
        float var16 = tp.y + 0.5f + exits[0].y * 0.5f;
        float var18 = tp.z + 0.5f + exits[0].z * 0.5f;
        float var20 = tp.x + 0.5f + exits[1].x * 0.5f;
        float var22 = tp.y + 0.5f + exits[1].y * 0.5f;
        float var24 = tp.z + 0.5f + exits[1].z * 0.5f;
        float var26 = var20 - var14;
        float var28 = (var22 - var16) * 2.0f;
        float var30 = var24 - var18;
        if (var26 == 0.0f) 
        {
            newPos->x = tp.x + 0.5f;
            var12 = newPos->z - tp.z;
        }
        else if (var30 == 0.0f) 
        {
            newPos->z = tp.z + 0.5f;
            var12 = newPos->x - tp.x;
        }
        else 
        {
            float var32 = newPos->x - var14;
            float var34 = newPos->z - var18;
            float var36 = (var32 * var26 + var34 * var30) * 2.0f;
            var12 = var36;
        }

        newPos->x = var14 + var26 * var12;
        newPos->y = var16 + var28 * var12;
        newPos->z = var18 + var30 * var12;
        if (var28 < 0.0f)
            ++newPos->y;

        if (var28 > 0.0f)
            newPos->y += 0.5f;

        return newPos;
    }
   
    return nullptr;
}

void Minecart::push(Entity* ent)
{
    if (m_pLevel->m_bIsClientSide || ent == getRider()) return;
    
    if (m_type == TYPE_DEFAULT
        && (m_vel.x * m_vel.x + m_vel.z * m_vel.z) > 0.01f
        && ent->getDescriptor().hasCategory(EntityCategories::MOB)
        && !ent->isPlayer()
        && !getRider()
        && !getRiding())
        ent->ride(this);

    float xDiff = ent->m_pos.x - m_pos.x;
    float zDiff = ent->m_pos.z - m_pos.z;
    float distSq = xDiff * xDiff + zDiff * zDiff;
    if (distSq >= 0.0001f) // non-zero?
    {
        float distSqrt = Mth::sqrt(distSq);
        xDiff /= distSqrt;
        zDiff /= distSqrt;

        float pushScale = 1.0f / distSqrt;
        if (pushScale > 1.0f)
            pushScale = 1.0f;

        xDiff *= pushScale;
        xDiff *= 0.1f;
        xDiff *= 1.0f - m_pushthrough;
        xDiff *= 0.5f;

        zDiff *= pushScale;
        zDiff *= 0.1f;
        zDiff *= 1.0f - m_pushthrough;
        zDiff *= 0.5f;

        if (ent->getDescriptor().hasCategory(EntityCategories::MINECART))
        {
            Minecart* other = (Minecart*)ent;
            float otherXDiff = ent->m_pos.x - m_pos.x;
            float otherZDiff = ent->m_pos.z - m_pos.z;

            float relativeMotion = otherXDiff * other->m_vel.z + otherZDiff * other->m_oPos.x;
            relativeMotion *= relativeMotion;
            if (relativeMotion > 5.0f)
                return;

            if (other->m_type == TYPE_FURNACE && m_type != TYPE_FURNACE)
            {
                m_vel.x *= 0.2f;
                m_vel.z *= 0.2f;
                Entity::push(Vec3(ent->m_vel.x - xDiff, 0.0f, ent->m_vel.z - zDiff));
                ent->m_vel.x *= 0.7f;
                ent->m_vel.z *= 0.7f;
            }
            else if (other->m_type != TYPE_FURNACE && m_type == TYPE_FURNACE)
            {
                ent->m_vel.x *= 0.2f;
                ent->m_vel.z *= 0.2f;
                ent->push(Vec3(m_vel.x + xDiff, 0.0f, m_vel.z + zDiff));
                m_vel.x *= 0.7f;
                m_vel.z *= 0.7f;
            }
            else
            {
                float otherProjectedX = ent->m_vel.x + m_vel.x;
                float otherProjectedZ = ent->m_vel.z + m_vel.z;
                otherProjectedX /= 2.0f;
                otherProjectedZ /= 2.0f;
                m_vel.x *= 0.2f;
                m_vel.z *= 0.2f;
                Entity::push(Vec3(otherProjectedX - xDiff, 0.0f, otherProjectedZ - zDiff));
                ent->m_vel.x *= 0.2f;
                ent->m_vel.z *= 0.2f;
                ent->push(Vec3(otherProjectedX + xDiff, 0.0f, otherProjectedZ + zDiff));
            }
        }
        else
        {
            Entity::push(Vec3(-xDiff, 0.0f, -zDiff));
            ent->push(Vec3(xDiff / 4.0f, 0.0f, zDiff / 4.0f));
        }
    }
}

void Minecart::lerpTo(const Vec3& pos, const Rot2& rot, int steps)
{
    m_lPos = pos;
    m_lRot = rot;
    m_lSteps = steps + 2;
    m_vel = m_lPosD;
}

void Minecart::lerpMotion(const Vec3& motion) 
{
    m_lPosD = m_vel = motion;
}

bool Minecart::stillValid(Player& player) const
{
    return !m_bRemoved && !(player.distanceToSqr(this) > 64.0f);
}
