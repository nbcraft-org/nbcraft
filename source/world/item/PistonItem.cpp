#include "PistonItem.hpp"

PistonItem::PistonItem(int id) : TileItem(id)
{
}

TileData PistonItem::getLevelDataForAuxValue(int x) const
{
	return 7;
}
