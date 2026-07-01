#include "MinecartItem.hpp"
#include "world/level/Level.hpp"
#include <world/tile/RailTile.hpp>

MinecartItem::MinecartItem(int id, Minecart::Type type) : Item(id)
{
	m_maxStackSize = 1;
	m_type = type;
}

bool MinecartItem::useOn(ItemStack* instance, Player* player, Level* level, const TilePos& pos, Facing::Name face) const
{
    if (RailTile::isRail(level->getTile(pos)))
    {
        if (!level->m_bIsClientSide)
            level->addEntity(new Minecart(level, Vec3(pos.x + 0.5f, float(pos.y), pos.z + 0.5f), m_type));

        --instance->m_count;
        return true;
    }
    else {
        return false;
    }
}
