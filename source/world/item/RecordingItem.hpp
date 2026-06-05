#pragma once

#include "Item.hpp"

class RecordingItem : public Item
{
public:
	RecordingItem(int id, const std::string& recording);

	bool useOn(ItemStack*, Player*, Level*, const TilePos& pos, Facing::Name face) const override;
private:
	std::string m_recording;
};
