#pragma once

#include "Item.hpp"

class BucketItem : public Item
{
public:
	BucketItem(int id, TileID content);
	bool use(ItemStack&, Level*, Mob&) const override;
protected:
	TileID m_content;
};
