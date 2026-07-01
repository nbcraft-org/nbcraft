#pragma once

#include "Item.hpp"

class RecordingItem : public Item
{
public:
	RecordingItem(int id, const std::string& recording);

public:
	bool useOn(ItemStack&, Player&, const TilePos& pos, Facing::Name face) const override;

private:
	std::string m_recording;
};
