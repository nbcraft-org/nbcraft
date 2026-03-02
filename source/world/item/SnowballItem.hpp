#pragma once

#include "Item.hpp"

class SnowballItem : public Item
{
public:
	SnowballItem(int id);

	ItemStack* use(ItemStack* inst, Level* level, Mob* player) const override;
};