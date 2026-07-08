#include "Minecart.hpp"
#include "world/entity/Player.hpp"
#include "world/level/Level.hpp"
#include "world/tile/RailTile.hpp"

const int Minecart::EXITS[10][2][3] = { {{0, 0, -1}, {0, 0, 1}}, {{-1, 0, 0}, {1, 0, 0}}, {{-1, -1, 0}, {1, 0, 0}}, {{-1, 0, 0}, {1, -1, 0}}, {{0, 0, -1}, {0, -1, 1}}, {{0, -1, -1}, {0, 0, 1}}, {{0, 0, 1}, {1, 0, 0}}, {{0, 0, 1}, {-1, 0, 0}}, {{0, 0, -1}, {-1, 0, 0}}, {{0, 0, -1}, {1, 0, 0}} };

void Minecart::_init()
{
    m_pDescriptor = &EntityTypeDescriptor::minecartRideable;
    m_renderType = RENDER_MINECART;
    m_lPos = Vec3::ZERO;
    m_lRot = Rot2::ZERO;
    m_lPosD = Vec3::ZERO;
    m_lSteps = 0;
    m_xPush = 0;
    m_zPush = 0;
    m_fuel = 0;
    m_type = Type::DEFAULT;
    m_damage = 0;
    m_hurtTime = 0;
    m_hurtDir = 1;
    m_bFlipped = false;
    m_bBlocksBuilding = true;
    setSize(0.98f, 0.7f);
    m_heightOffset = m_bbHeight / 2.0f;
    m_bMakeStepSound = false;
}

Minecart::Minecart(Level* level)
    : Entity(level)
    , SimpleContainer(27, "gui.chestCart")
{
    _init();
}

Minecart::Minecart(Level* level, const Vec3& pos, Type type) 
    : Entity(level)
    , SimpleContainer(27, "gui.chestCart")
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
    if (m_type == Minecart::DEFAULT)
    {
        if (getRider() && getRider()->isPlayer() && getRider() != player)
            return true;

        if (!m_pLevel->m_bIsClientSide) 
            player->ride(this);
    }
    else if (m_type == Minecart::CHEST) 
    {
        if (!m_pLevel->m_bIsClientSide) 
            player->openContainer(this);
    }
    else if (m_type == Minecart::FURNACE)
    {
        ItemStack& var2 = player->m_pInventory->getSelected();
        if (var2 && var2.getId() == Item::coal->m_itemID)
        {
            if (--var2.m_count == 0)
                player->m_pInventory->setSelectedItem(ItemStack::EMPTY);

            m_fuel += 1200;
        }

        m_xPush = m_pos.x - player->m_pos.x;
        m_zPush = m_pos.z - player->m_pos.z;
    }

    return true;
}

void Minecart::tick()
{
    if (m_hurtTime > 0) 
        --m_hurtTime;

    if (m_damage > 0)
        --m_damage;

    float var7;
    if (m_pLevel->m_bIsClientSide && m_lSteps > 0) 
    {
        //@NOTE: this statement doesn't really make sense, it'll always be true...
        if (m_lSteps > 0)
        {
            Vec3 newPos = m_pos + (m_lPos - m_pos) / m_lSteps;

            for (var7 = m_lRot.yaw - m_rot.yaw; var7 < -180.0f; var7 += 360.0f) {
            }

            while (var7 >= 180.0f) {
                var7 -= 360.0f;
            }

            m_rot.yaw = m_rot.yaw + var7 / m_lSteps;
            m_rot.pitch = m_rot.pitch + (m_lRot.pitch - m_rot.pitch) / m_lSteps;
            --m_lSteps;
            setPos(newPos);
            setRot(m_rot);
        }
        else {
            setPos(m_pos);
            setRot(m_rot);
        }

    }
    else 
    {
        m_oPos = m_pos;
        m_vel.y -= 0.04f;
        TilePos tp(m_pos);
        if (RailTile::isRail(m_pLevel->getTile(tp.below())))
            --tp.y;
     
        float var4 = 0.4f;
        bool canPush = false;
        var7 = 0.0078125f;
        TileID tile = m_pLevel->getTile(tp);
        if (RailTile::isRail(tile))
        {
            Vec3* var9 = getPos(m_pos);
            TileData data = m_pLevel->getData(tp);
            m_pos.y = tp.y;

            bool var49 = false;
            bool var13 = false;
            if (RailTile::isPoweredRail(Tile::tiles[tile])) 
            {
                var49 = (data & 8) != 0;
                var13 = !var49;
            }

            if (RailTile::isPowered(Tile::tiles[tile]))
                data &= 7;

            if (data >= 2 && data <= 5)
                m_pos.y = tp.y + 1;

            if (data == 2)
                m_vel.x -= var7;
            else if (data == 3)
                m_vel.x += var7;
            else if (data == 4)
                m_vel.z += var7;
            else if (data == 5)
                m_vel.z -= var7;

            const int (*exits)[3] = EXITS[data];

            float var12 = exits[1][0] - exits[0][0];
            float var14 = exits[1][2] - exits[0][2];
            float var16 = Mth::sqrt(var12 * var12 + var14 * var14);
            float var18 = m_vel.x * var12 + m_vel.z * var14;
            if (var18 < 0.0f)
            {
                var12 = -var12;
                var14 = -var14;
            }

            float var20 = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
            m_vel.x = var20 * var12 / var16;
            m_vel.z = var20 * var14 / var16;
            float var22;
            if (var13) 
            {
                var22 = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
                if (var22 < 0.03)
                {
                    m_vel *= 0;
                }
                else
                {
                    m_vel.x *= 0.5;
                    m_vel.y *= 0.0f;
                    m_vel.z *= 0.5;
                }
            }
            var22 = 0.0f;
            float var24 = tp.x + 0.5f + exits[0][0] * 0.5f;
            float var26 = tp.z + 0.5f + exits[0][2] * 0.5f;
            float var28 = tp.x + 0.5f + exits[1][0] * 0.5f;
            float var30 = tp.z + 0.5f + exits[1][2] * 0.5f;
            var12 = var28 - var24;
            var14 = var30 - var26;
            float var32;
            float var34;
            float var36;
            if (var12 == 0.0f)
            {
                m_pos.x = tp.x + 0.5;
                var22 = m_pos.z - tp.z;
            }
            else if (var14 == 0.0f)
            {
                m_pos.z = tp.z + 0.5;
                var22 = m_pos.x - tp.x;
            }
            else {
                var32 = m_pos.x - var24;
                var34 = m_pos.z - var26;
                var36 = (var32 * var12 + var34 * var14) * 2.0;
                var22 = var36;
            }

            m_pos.x = var24 + var12 * var22;
            m_pos.z = var26 + var14 * var22;
            setPos(Vec3(m_pos.x, m_pos.y + m_heightOffset, m_pos.z));
            var32 = m_vel.x;
            var34 = m_vel.z;
            if (getRider()) 
            {
                var32 *= 0.75;
                var34 *= 0.75;
            }

            if (var32 < -var4)
                var32 = -var4;

            if (var32 > var4)
                var32 = var4;

            if (var34 < -var4)
                var34 = -var4;

            if (var34 > var4)
                var34 = var4;

            move(Vec3(var32, 0.0f, var34));
            if (exits[0][1] != 0 && Mth::floor(m_pos.x) - tp.x == exits[0][0] && Mth::floor(m_pos.z) - tp.z == exits[0][2]) {
                setPos(Vec3(m_pos.x, m_pos.y + exits[0][1], m_pos.z));
            }
            else if (exits[1][1] != 0 && Mth::floor(m_pos.x) - tp.x == exits[1][0] && Mth::floor(m_pos.z) - tp.z == exits[1][2]) {
                setPos(Vec3(m_pos.x, m_pos.y + exits[1][1], m_pos.z));
            }

            if (getRider()) 
            {
                m_vel.x *= 0.997;
                m_vel.y *= 0.0f;
                m_vel.z *= 0.997;
            }
            else 
            {
                if (m_type == Type::FURNACE) 
                {
                    var36 = Mth::sqrt(m_xPush * m_xPush + m_zPush * m_zPush);
                    if (var36 > 0.01) 
                    {
                        canPush = true;
                        m_xPush /= var36;
                        m_zPush /= var36;
                        float var38 = 0.04;
                        m_vel.x *= 0.8;
                        m_vel.y *= 0.0f;
                        m_vel.z *= 0.8;
                        m_vel.x += m_xPush * var38;
                        m_vel.z += m_zPush * var38;
                    }
                    else 
                    {
                        m_vel.x *= 0.9;
                        m_vel.y *= 0.0f;
                        m_vel.z *= 0.9;
                    }
                }

                m_vel.x *= 0.96f;
                m_vel.y *= 0.0f;
                m_vel.z *= 0.96f;
            }

            if (var49) 
            {
                var36 = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
                if (var36 > 0.01) 
                {
                    constexpr float var44 = 0.06;
                    m_vel.x += m_vel.x / var36 * var44;
                    m_vel.z += m_vel.z / var36 * var44;
                }
                else if (data == 1)
                {
                    if (m_pLevel->isSolidBlockingTile(tp.west()))
                    {
                        m_vel.x = 0.02;
                    }
                    else if (m_pLevel->isSolidBlockingTile(tp.east()))
                    {
                        m_vel.x = -0.02;
                    }
                }
                else if (data == 0)
                {
                    if (m_pLevel->isSolidBlockingTile(tp.north()))
                    {
                        m_vel.z = 0.02;
                    }
                    else if (m_pLevel->isSolidBlockingTile(tp.south()))
                    {
                        m_vel.z = -0.02;
                    }
                }
            }

            Vec3* var46 = getPos(m_pos);
            if (var46 && var9) 
            {
                float var37 = (var9->y - var46->y) * 0.05;
                var20 = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
                if (var20 > 0.0f) 
                {
                    m_vel.x = m_vel.x / var20 * (var20 + var37);
                    m_vel.z = m_vel.z / var20 * (var20 + var37);
                }

                setPos(Vec3(m_pos.x, var46->y, m_pos.z));
     
            }
            SAFE_DELETE(var9);
            SAFE_DELETE(var46);

            int var47 = Mth::floor(m_pos.x);
            int var48 = Mth::floor(m_pos.z);
            if (var47 != tp.x || var48 != tp.z)
            {
                var20 = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
                m_vel.x = var20 * (var47 - tp.x);
                m_vel.z = var20 * (var48 - tp.z);
            }

            if (m_type == Type::FURNACE)
            {
                float var39 = Mth::sqrt(m_xPush * m_xPush + m_zPush * m_zPush);
                if (var39 > 0.01 && m_vel.x * m_vel.x + m_vel.z * m_vel.z > 0.001) {
                    m_xPush /= var39;
                    m_zPush /= var39;
                    if (m_xPush * m_vel.x + m_zPush * m_vel.z < 0.0f) {
                        m_xPush = 0.0f;
                        m_zPush = 0.0f;
                    }
                    else {
                        m_xPush = m_vel.x;
                        m_zPush = m_vel.z;
                    }
                }
            }
        }
        else
        {
            if (m_vel.x < -var4)
                m_vel.x = -var4;

            if (m_vel.x > var4)
                m_vel.x = var4;

            if (m_vel.z < -var4)
                m_vel.z = -var4;

            if (m_vel.z > var4)
                m_vel.z = var4;

            if (m_bOnGround)
            {
                m_vel *= 0.5f;
            }

            move(m_vel);
            if (!m_bOnGround) 
            {
                m_vel *= 0.95f;
            }
        }

        m_rot.pitch = 0.0f;
        float var43 = m_oPos.x - m_pos.x;
        float var44 = m_oPos.z - m_pos.z;
        if (var43 * var43 + var44 * var44 > 0.001f)
        {
            m_rot.yaw = Mth::atan2(var44, var43) * 180.0f / M_PI;
            if (m_bFlipped)
                m_rot.yaw += 180.0f;
        }

        float var13;
        for (var13 = m_rot.yaw - m_oRot.yaw; var13 >= 180.0f; var13 -= 360.0f) {
        }

        while (var13 < -180.0f)
            var13 += 360.0f;

        if (var13 < -170.0f || var13 >= 170.0f) 
        {
            m_rot.yaw += 180.0f;
            m_bFlipped = !m_bFlipped;
        }

        setRot(m_rot);
        AABB hit = m_hitbox;
        hit.grow(0.2, 0.0f, 0.2);
        EntityVector var15 = m_pLevel->getEntities(this, hit);
        if (var15.size() > 0) 
        {
            for (EntityVector::iterator it = var15.begin(); it != var15.end(); ++it)
            {
                Entity* entity = *it;
                if (entity != getRider() && entity->isPushable() && entity->getDescriptor().hasCategory(EntityCategories::MINECART))
                    entity->push(this);
            }
        }

        if (getRider() && getRider()->m_bRemoved)
            setRider(nullptr);

        if (canPush && m_random.nextInt(4) == 0) 
        {
            --m_fuel;
            if (m_fuel < 0)
                m_xPush = m_zPush = 0.0f;

            m_pLevel->addParticle("largesmoke", Vec3(m_pos.x, m_pos.y + 0.8, m_pos.z));
        }

    }
}

float Minecart::getRideHeight() const
{
	return -0.3f;
}

bool Minecart::hurt(Entity*, int mul)
{
    if (!m_pLevel->m_bIsClientSide && !m_bRemoved)
    {
        m_hurtDir = -m_hurtDir;
        m_hurtTime = 10;
        markHurt();
        m_damage += mul * 10;
        if (m_damage > 40)
        {
            spawnAtLocation(Item::minecart->m_itemID, 1, 0.0f);
            if (m_type == 1)
            {
                spawnAtLocation(Tile::chest->m_ID, 1, 0.0f);
            }
            else if (m_type == 2)
            {
                spawnAtLocation(Tile::furnace->m_ID, 1, 0.0f);
            }

            remove();
        }
        return true;
    }
    else
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
    for (int var1 = 0; var1 < getContainerSize(); ++var1)
    {
        ItemStack& var2 = getItem(var1);
        if (!var2.isEmpty())
        {
            float var3 = m_random.nextFloat() * 0.8f + 0.1f;
            float var4 = m_random.nextFloat() * 0.8f + 0.1f;
            float var5 = m_random.nextFloat() * 0.8f + 0.1f;

            while (var2.m_count > 0)
            {
                int var6 = m_random.nextInt(21) + 10;
                if (var6 > var2.m_count) {
                    var6 = var2.m_count;
                }

                var2.m_count -= var6;
                ItemEntity* var7 = new ItemEntity(m_pLevel, Vec3(m_pos.x + var3, m_pos.y + var4, m_pos.z + var5), ItemStack(var2.getId(), var6, var2.getAuxValue()));
                float var8 = 0.05f;
                var7->m_vel.x = m_random.nextGaussian() * var8;
                var7->m_vel.y = m_random.nextGaussian() * var8 + 0.2f;
                var7->m_vel.z = m_random.nextGaussian() * var8;
                m_pLevel->addEntity(var7);
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

    int tile = m_pLevel->getTile(tp);

    if (RailTile::isRail(tile))
    {
        Vec3 newPos = pos;
        TileData data = m_pLevel->getData(tp);
        newPos.y = tp.y;

        if (RailTile::isPowered(Tile::tiles[tile]))
            data &= 7;

        if (data >= 2 && data <= 5)
            newPos.y = tp.y + 1;

        const int (*exits)[3] = EXITS[data];

        float var14 = (exits[1][0] - exits[0][0]);
        float var16 = (exits[1][2] - exits[0][2]);
        float var18 = Mth::sqrt(var14 * var14 + var16 * var16);
        var14 /= var18;
        var16 /= var18;
        newPos.x += var14 * var7;
        newPos.z += var16 * var7;
        if (exits[0][1] != 0 && Mth::floor(pos.x) - tp.x == exits[0][0] && Mth::floor(newPos.z) - tp.z == exits[0][2])
            newPos.y += exits[0][1];
        else if (exits[1][1] != 0 && Mth::floor(pos.x) - tp.x == exits[1][0] && Mth::floor(newPos.z) - tp.z == exits[1][2])
            newPos.y += exits[1][1];

        return getPos(newPos);
    }
        
    return nullptr;
}

void Minecart::addAdditionalSaveData(CompoundTag& tag) const
{
    tag.putInt8("Type", m_type);
    if (m_type == Type::FURNACE) 
    {
        tag.putFloat("PushX", m_xPush);
        tag.putFloat("PushZ", m_zPush);
        tag.putInt16("Fuel", m_fuel);
    } else if (m_type == Type::CHEST) 
        SimpleContainer::save(tag);
}

void Minecart::readAdditionalSaveData(const CompoundTag& tag)
{
    m_type = (Type) tag.getInt8("Type");
    if (m_type == Type::FURNACE) 
    {
        m_xPush = tag.getDouble("PushX");
        m_zPush = tag.getDouble("PushZ");
        m_fuel = tag.getInt16("Fuel");
    }
    else if (m_type == Type::CHEST)
        SimpleContainer::load(tag);
}

float Minecart::getLootContent()
{
    int var1 = 0;

    for (int var2 = 0; var2 < getContainerSize(); ++var2)
    {
        if (getItem(var2))
            ++var1;
    }

    return (float)var1 / getContainerSize();
}

Vec3* Minecart::getPos(const Vec3& vec) const
{
    TilePos tp(vec);
    if (RailTile::isRail(m_pLevel->getTile(tp.below())))
        --tp.y;

    int tile = m_pLevel->getTile(tp);
    if (RailTile::isRail(tile))
    {
        Vec3* newPos = new Vec3(vec);
        TileData data = m_pLevel->getData(tp);
        newPos->y = tp.y;

        if (RailTile::isPowered(Tile::tiles[tile]))
            data &= 7;

        if (data >= 2 && data <= 5)
            newPos->y = (tp.y + 1);

        const int (*exits)[3] = EXITS[data];

        float var12 = 0.0f;
        float var14 = tp.x + 0.5f + exits[0][0] * 0.5f;
        float var16 = tp.y + 0.5f + exits[0][1] * 0.5f;
        float var18 = tp.z + 0.5f + exits[0][2] * 0.5f;
        float var20 = tp.x + 0.5f + exits[1][0] * 0.5f;
        float var22 = tp.y + 0.5f + exits[1][1] * 0.5f;
        float var24 = tp.z + 0.5f + exits[1][2] * 0.5f;
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
    
    if (ent->getDescriptor().hasCategory(EntityCategories::MOB) && !ent->isPlayer() && m_type == DEFAULT && (m_vel.x * m_vel.x + m_vel.z * m_vel.z) > 0.01f && !getRider() && !getRiding())
        ent->ride(this);

    float var2 = ent->m_pos.x - m_pos.x;
    float var4 = ent->m_pos.z - m_pos.z;
    float var6 = var2 * var2 + var4 * var4;
    if (var6 >= 1.0E-4f)
    {
        var6 = Mth::sqrt(var6);
        var2 /= var6;
        var4 /= var6;
        float var8 = 1.0f / var6;
        if (var8 > 1.0f)
            var8 = 1.0f;

        var2 *= var8;
        var4 *= var8;
        var2 *= 0.1;
        var4 *= 0.1;
        var2 *= 1.0f - m_pushThrough;
        var4 *= 1.0f - m_pushThrough;
        var2 *= 0.5;
        var4 *= 0.5;
        if (ent->getDescriptor().hasCategory(EntityCategories::MINECART))
        {
            Minecart* minecart = (Minecart*)ent;
            float xDiff = ent->m_pos.x - m_pos.x;
            float zDiff = ent->m_pos.z - m_pos.z;

            float var14 = xDiff * minecart->m_vel.z + zDiff * minecart->m_oPos.x;
            var14 *= var14;
            if (var14 > 5.0f)
                return;

            float var10 = ent->m_vel.x + m_vel.x;
            float var12 = ent->m_vel.z + m_vel.z;
            if (minecart->m_type == FURNACE && m_type != FURNACE)
            {
                m_vel.x *= 0.2f;
                m_vel.z *= 0.2f;
                Entity::push(Vec3(ent->m_vel.x - var2, 0.0f, ent->m_vel.z - var4));
                ent->m_vel.x *= 0.7f;
                ent->m_vel.z *= 0.7f;
            }
            else if (minecart->m_type != FURNACE && m_type == FURNACE)
            {
                ent->m_vel.x *= 0.2;
                ent->m_vel.z *= 0.2;
                ent->push(Vec3(m_vel.x + var2, 0.0f, m_vel.z + var4));
                m_vel.x *= 0.7f;
                m_vel.z *= 0.7f;
            }
            else
            {
                var10 /= 2.0f;
                var12 /= 2.0f;
                m_vel.x *= 0.2f;
                m_vel.z *= 0.2f;
                Entity::push(Vec3(var10 - var2, 0.0f, var12 - var4));
                ent->m_vel.x *= 0.2f;
                ent->m_vel.z *= 0.2f;
                ent->push(Vec3(var10 + var2, 0.0f, var12 + var4));
            }
        }
        else
        {
            Entity::push(Vec3(-var2, 0.0f, -var4));
            ent->push(Vec3(var2 / 4.0f, 0.0f, var4 / 4.0f));
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

bool Minecart::stillValid(Player* var1) const
{
    return !m_bRemoved && !(var1->distanceToSqr(this) > 64.0f);
}
