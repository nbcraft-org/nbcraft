#pragma once

#include "Item.hpp"

class SnowballItem : public Item
{
public:
	SnowballItem(int id);

	ItemInstance* use(ItemInstance* inst, Level* level, Player* player) const override;
};