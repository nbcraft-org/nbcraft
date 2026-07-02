#pragma once

#include "TileItem.hpp"

class PistonItem : public TileItem
{
public:
	PistonItem(int id);

	TileData getLevelDataForAuxValue(int x) const override;
};
