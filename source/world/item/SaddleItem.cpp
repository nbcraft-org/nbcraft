#include "SaddleItem.hpp"
#include "world/entity/Mob.hpp"
#include <world/entity/Pig.hpp>

SaddleItem::SaddleItem(int id) : Item(id)
{
	m_maxStackSize = 1;
}

void SaddleItem::interactEnemy(ItemStack* stack, Mob* mob) const
{
    if (mob->getDescriptor().getEntityType() == EntityType::PIG)
    {
        Pig* pig = (Pig*)mob;
        if (!pig->hasSaddle())
        {
            pig->setSaddle(true);
            --stack->m_count;
        }
    }
}

bool SaddleItem::hurtEnemy(ItemStack* stack, Mob* mob, Mob* attacker) const
{
	interactEnemy(stack, mob);
    return true;
}
