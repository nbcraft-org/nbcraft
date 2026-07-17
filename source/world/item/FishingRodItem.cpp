#include "FishingRodItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/projectile/FishingHook.hpp"

FishingRodItem::FishingRodItem(int id) : Item(id)
{
	m_maxDamage = 64;
}

bool FishingRodItem::use(ItemStack& item, Level* level, Mob& user) const
{
	if (!user.isPlayer())
		return false;

	Player& player = static_cast<Player&>(user);
	if (player.m_pFishing)
	{
		int var4 = player.m_pFishing->retrieve();
		item.hurt(var4);
		user.swing();
	}
	else
	{
		level->playSound(&user, "random.bow", 0.5f, 0.4f / (random.nextFloat() * 0.4f + 0.8f));
		if (!level->m_bIsClientSide)
		{
			level->addEntity(new FishingHook(level, &player));
		}

		user.swing();
	}

	return true;
}
