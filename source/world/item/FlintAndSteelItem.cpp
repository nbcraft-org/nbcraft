#include "FlintAndSteelItem.hpp"
#include "world/entity/Player.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"

FlintAndSteelItem::FlintAndSteelItem(int itemID) : Item(itemID)
{
	m_maxStackSize = 16;
}

bool FlintAndSteelItem::useOn(ItemStack& itemStack, Player& player, const TilePos& pos, Facing::Name face) const
{
    Level& level = player.getLevel();
    TileSource& tileSource = player.getTileSource();
    TilePos targetPos = pos.relative(face);

    int i = tileSource.getTile(targetPos);
    if (i == 0)
    {
        level.playSound(Vec3((float)targetPos.x + 0.5f, (float)targetPos.y + 0.5f, (float)targetPos.z + 0.5f), "fire.ignite", 1.0f, level.m_random.nextFloat() * 0.4f + 0.8f);
        tileSource.setTile(targetPos, Tile::fire->m_ID);
    }
        
    itemStack.hurtAndBreak(1, player);
    return true;
}
