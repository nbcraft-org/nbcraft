#pragma once

#include "Item.hpp"
#include "world/entity/Minecart.hpp"

class MinecartItem : public Item
{
public:
	MinecartItem(int id, Minecart::Type type);

	bool useOn(ItemStack&, Player&, const TilePos& pos, Facing::Name face) const override;

public:
	Minecart::Type m_type;
};
