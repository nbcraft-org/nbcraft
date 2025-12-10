#pragma once

#include "Item.hpp"

class RedStoneItem : public Item
{
public:
	RedStoneItem(int id);

	bool useOn(ItemInstance*, Player*, Level*, const TilePos& pos, Facing::Name face) const override;
};