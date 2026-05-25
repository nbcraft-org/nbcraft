#pragma once

#include "Item.hpp"

class EggItem : public Item
{
public:
	EggItem(int);

public:
	ItemStack* use(ItemStack* inst, Level* level, Mob* user) const;
};
