#pragma once

#include "Item.hpp"

class EggItem : public Item
{
public:
	EggItem(int);

public:
	bool use(ItemStack&, Level*, Mob&) const override;
};
