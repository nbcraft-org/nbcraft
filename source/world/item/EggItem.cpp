#include "EggItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/ThrownEgg.hpp"

EggItem::EggItem(int id) : Item(id)
{
	m_maxStackSize = 16;
}

ItemInstance* EggItem::use(ItemInstance* inst, Level* level, Player* player) const
{
	--inst->m_count;
	level->playSound(player, "random.bow", 0.5f, 0.4f / (m_random.nextFloat() * 0.4f + 0.8f));
	if (!level->m_bIsClientSide) {
		level->addEntity(new ThrownEgg(level, player));
	}

	return inst;
}
