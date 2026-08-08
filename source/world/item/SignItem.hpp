#pragma once

#include "Item.hpp"

class SignItem : public Item
{
public:
	SignItem(int id);

	bool useOn(ItemStack*, Player*, Level*, const TilePos& pos, Facing::Name face) const override;
};
