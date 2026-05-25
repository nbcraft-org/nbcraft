#pragma once

#include "Item.hpp"

class SnowballItem : public Item
{
public:
	SnowballItem(int);

public:
	ItemStack* use(ItemStack* inst, Level* level, Mob* user) const;
};
