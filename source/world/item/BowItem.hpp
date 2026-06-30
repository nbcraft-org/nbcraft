#pragma once

#include "Item.hpp"

class BowItem : public Item
{
public:
	BowItem(int id);

public:
	bool use(ItemStack&, Mob&) const override;
};
