#pragma once

#include "Item.hpp"

class EggItem : public Item
{
public:
	EggItem(int id);

	ItemStack* use(ItemStack* inst, Level* level, Mob* player) const override;
};