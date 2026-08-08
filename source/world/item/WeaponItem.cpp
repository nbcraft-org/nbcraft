#include "WeaponItem.hpp"
#include "world/entity/Mob.hpp"
#include "world/entity/Player.hpp"

WeaponItem::WeaponItem(int id, Tier& tier) : ToolItem(id, Tool::SWORD, tier), m_attackDamage(4 + tier.m_damage)
{
}

float WeaponItem::getDestroySpeed(ItemStack* instance, const Tile* tile) const
{
	return (_canDestroyTile(tile) || _canDestroyMaterial(tile->m_pMaterial)) ? 15.0f : 1.5f;
}

bool WeaponItem::hurtEnemy(ItemStack* instance, Mob* mob, Mob* attacker) const
{
	instance->hurtAndBreak(1, mob);
	return true;
}

bool WeaponItem::mineBlock(ItemStack* instance, const TilePos& pos, Facing::Name face, Mob* mob) const
{
	instance->hurtAndBreak(2, mob);
	return true;
}

int WeaponItem::getAttackDamage(Entity* entity) const
{
	return m_attackDamage;
}