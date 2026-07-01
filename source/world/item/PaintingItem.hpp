#pragma once

#include "Item.hpp"

class PaintingItem : public Item
{
public:
	PaintingItem(int id);

	bool useOn(ItemStack* instance, Player* player, Level* level, const TilePos& pos, Facing::Name face) const override;
};
