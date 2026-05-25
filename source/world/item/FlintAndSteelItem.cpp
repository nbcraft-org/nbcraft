#include "FlintAndSteelItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"

FlintAndSteelItem::FlintAndSteelItem(int itemID) : Item(itemID)
{
	m_maxStackSize = 16;
}

bool FlintAndSteelItem::useOn(ItemStack* instance, Player* player, Level* level, const TilePos& pos, Facing::Name face) const
{
    TilePos targetPos = pos;

    switch (face) {
    case Facing::DOWN:    targetPos.y--; break;
    case Facing::UP:      targetPos.y++; break;
    case Facing::NORTH:   targetPos.z--; break;
    case Facing::SOUTH:   targetPos.z++; break;
    case Facing::WEST:    targetPos.x--; break;
    case Facing::EAST:    targetPos.x++; break;
    }

    int i = level->getTile(targetPos);
    if (i == 0) {
        level->playSound(Vec3((float)targetPos.x + 0.5f, (float)targetPos.y + 0.5f, (float)targetPos.z + 0.5f), "fire.ignite", 1.0f, level->m_random.nextFloat() * 0.4f + 0.8f);
        level->setTile(targetPos, Tile::fire->m_ID);
    }
        
    instance->hurtAndBreak(1, player);
    return true;
}
