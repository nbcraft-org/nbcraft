#include "FishingRodItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/FishingHook.hpp"

FishingRodItem::FishingRodItem(int id) : Item(id)
{
	m_maxDamage = 64;
}

ItemStack* FishingRodItem::use(ItemStack* inst, Level* level, Mob* mob) const
{
	Player* player = reinterpret_cast<Player*>(mob);
	if (player->m_pFishing)
	{
		int var4 = player->m_pFishing->retrieve();
		inst->hurt(var4);
		mob->swing();
	}
	else
	{
		level->playSound(mob, "random.bow", 0.5f, 0.4f / (random.nextFloat() * 0.4f + 0.8f));
		if (!level->m_bIsClientSide) {
			level->addEntity(new FishingHook(level, player));
		}

		mob->swing();
	}

	return inst;
}
