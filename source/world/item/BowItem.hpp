#pragma once

#include "Item.hpp"

class BowItem : public Item
{
public:
	BowItem(int id);

public:
	bool use(ItemStack&, Level*, Mob&) const override;
};
