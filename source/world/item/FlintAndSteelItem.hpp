#pragma once

#include "Item.hpp"

class FlintAndSteelItem : public Item
{
public:
	FlintAndSteelItem(int);

public:
	bool useOn(ItemStack* instance, Player* player, Level* level, const TilePos& pos, Facing::Name face) const;
};
