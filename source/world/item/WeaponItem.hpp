#pragma once

#include "ToolItem.hpp"

class WeaponItem : public ToolItem
{
public:
	WeaponItem(int id, Tier& tier);

public:
	float getDestroySpeed(ItemStack* instance, const Tile* tile) const override;
	bool hurtEnemy(ItemStack* instance, Mob* mob, Mob* attacker) const override;
	bool mineBlock(ItemStack* instance, const TilePos& pos, Facing::Name face, Mob* mob) const override;
	int getAttackDamage(Entity*) const override;

private:
	int m_attackDamage;
};
