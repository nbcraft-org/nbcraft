#include "FishingRodItem.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/FishingHook.hpp"
#include "world/level/Level.hpp"

FishingRodItem::FishingRodItem(int id) : Item(id)
{
	m_maxDamage = 64;
}

bool FishingRodItem::use(ItemStack& item, Mob& user) const
{
	if (!user.isPlayer())
		return false;

	Level& level = user.getLevel();
	Player& player = static_cast<Player&>(user);

	if (player.m_pFishing)
	{
		int var4 = player.m_pFishing->retrieve();
		item.hurt(var4);
		user.swing();
	}
	else
	{
		level.playSound(&user, "random.bow", 0.5f, 0.4f / (random.nextFloat() * 0.4f + 0.8f));
		if (!level.m_bIsClientSide)
		{
			level.addEntity(new FishingHook(player));
		}

		user.swing();
	}

	return true;
}
