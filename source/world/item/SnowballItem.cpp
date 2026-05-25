#include "SnowballItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Snowball.hpp"


SnowballItem::SnowballItem(int itemID) : Item(itemID)
{
	m_maxStackSize = 16;
}

ItemStack* SnowballItem::use(ItemStack* inst, Level* level, Mob* user) const 
{
	--inst->m_count;
	level->playSound(user, "random.bow", 0.5f, 0.4F / (level->m_random.nextFloat() * 0.4F + 0.8F));
	if (!level->m_bIsClientSide)
		level->addEntity(new Snowball(level, user));
	return inst;
}
