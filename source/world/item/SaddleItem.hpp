#pragma once

#include "Item.hpp"

class SaddleItem : public Item
{
public:
	SaddleItem(int id);
	void interactEnemy(ItemStack*, Mob* mob) const override;
};
