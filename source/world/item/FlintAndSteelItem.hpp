#pragma once

#include "Item.hpp"

class FlintAndSteelItem : public Item
{
public:
	FlintAndSteelItem(int);

public:
	bool useOn(ItemStack* instance, Player* player, const TilePos& pos, Facing::Name face) const override;
};
