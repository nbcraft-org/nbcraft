#include "FoodItem.hpp"
#include "world/entity/Mob.hpp"

FoodItem::FoodItem(int id, int nutrition) : Item(id), m_nutrition(nutrition)
{
	m_maxStackSize = 1;
}

bool FoodItem::use(ItemStack& inst, Level* level, Mob& user) const
{
    if (user.m_health < user.getMaxHealth())
    {
        --inst.m_count;
        user.heal(m_nutrition);
        return true;
    }
    return false;
}
