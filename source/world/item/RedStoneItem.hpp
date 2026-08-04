#pragma once

#include "Item.hpp"

class RedStoneItem : public Item
{
public:
	RedStoneItem(int id);

public:
	bool useOn(ItemStack&, Player&, const TilePos& pos, Facing::Name face) const override;
};
