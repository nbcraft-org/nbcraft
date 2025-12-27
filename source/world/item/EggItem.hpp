#pragma once

#include "Item.hpp"

class EggItem : public Item
{
public:
	EggItem(int id);

	ItemInstance* use(ItemInstance* inst, Level* level, Player* player) const override;
};