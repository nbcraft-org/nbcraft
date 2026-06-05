#pragma once

#include "Item.hpp"

class FishingRodItem : public Item
{
public:
	FishingRodItem(int id);

	bool isHandEquipped() const override { return true; }
	bool isMirroredArt() const override { return true; }
	ItemStack* use(ItemStack* inst, Level* level, Mob* player) const override;
};
