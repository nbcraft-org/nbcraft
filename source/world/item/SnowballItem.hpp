#pragma once

#include "Item.hpp"

class SnowballItem : public Item
{
public:
	SnowballItem(int);

public:
	bool use(ItemStack&, Level*, Mob&) const override;
};
