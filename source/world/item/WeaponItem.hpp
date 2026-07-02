#pragma once

#include "ToolItem.hpp"

class WeaponItem : public ToolItem
{
public:
	WeaponItem(int id, Tier& tier);

public:
	float getDestroySpeed(ItemStack& itemStack, const Tile* tile) const override;
	void hurtEnemy(ItemStack& itemStack, Mob& mob) const override;
	void mineBlock(ItemStack& itemStack, const TilePos& pos, Facing::Name face, Mob& mob) const override;
	int getAttackDamage(Entity&) const override;

private:
	int m_attackDamage;
};
