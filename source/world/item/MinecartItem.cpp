#include "MinecartItem.hpp"
#include "world/level/Level.hpp"
#include "world/tile/RailTile.hpp"

MinecartItem::MinecartItem(int id, Minecart::Type type) : Item(id)
{
	m_maxStackSize = 1;
	m_type = type;
}

bool MinecartItem::useOn(ItemStack& instance, Player& player, const TilePos& pos, Facing::Name face) const
{
    Level& level = player.getLevel();
    if (!RailTile::isRail(level.getTile(pos)))
        return false;

    if (!level.m_bIsClientSide)
        level.addEntity(new Minecart(level, m_type, Vec3(pos.x + 0.5f, float(pos.y), pos.z + 0.5f)));

    --instance.m_count;
    return true;
}
