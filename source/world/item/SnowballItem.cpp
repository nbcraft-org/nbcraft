#include "SnowballItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/Snowball.hpp"

SnowballItem::SnowballItem(int id) : Item(id)
{
	m_maxStackSize = 16;
}

ItemStack* SnowballItem::use(ItemStack* inst, Level* level, Mob* player) const
{
	--inst->m_count;
	level->playSound(player, "random.bow", 0.5f, 0.4f / (m_random.nextFloat() * 0.4f + 0.8f));
	if (!level->m_bIsClientSide) {
		level->addEntity(new Snowball(level, player));
	}

	return inst;
}
