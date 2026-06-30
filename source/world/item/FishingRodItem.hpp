#pragma once

#include "Item.hpp"

class FishingRodItem : public Item
{
public:
	FishingRodItem(int id);

public:
	bool isHandEquipped() const override { return true; }
	bool isMirroredArt() const override { return true; }
	bool use(ItemStack& item, Mob& user) const override;
};
