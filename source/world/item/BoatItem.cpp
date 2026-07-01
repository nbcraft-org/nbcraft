#include "BoatItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/vehicle/Boat.hpp"

BoatItem::BoatItem(int id) : Item(id)
{
	m_maxStackSize = 1;
}


bool BoatItem::use(ItemStack& item, Level* level, Mob& user) const
{
    Vec3 var13(user.m_pos.x, user.m_pos.y + 1.62 - user.m_heightOffset, user.m_pos.z);
    constexpr float r = M_PI / 180;
    float var14 = Mth::cos(-user.m_rot.yaw * r - M_PI);
    float var15 = Mth::sin(-user.m_rot.yaw * r - M_PI);
    float var16 = -Mth::cos(-user.m_rot.pitch * r);
    float var17 = Mth::sin(-user.m_rot.pitch * r);
    float var18 = var15 * var16;
    float var20 = var14 * var16;
    HitResult hitResult = level->clip(var13, var13.add(var18 * 5, var17 * 5, var20 * 5), true);
    if (!hitResult.isHit())
        return item;
    else
    {
        if (hitResult.m_hitType == HitResult::TILE)
        {
            if (!level->m_bIsClientSide)
                level->addEntity(new Boat(level, TilePos(hitResult.m_tilePos.x + 0.5f, hitResult.m_tilePos.y + 1.5f, hitResult.m_tilePos.z + 0.5f)));

            --item.m_count;
        }

        return item;
    }
}
