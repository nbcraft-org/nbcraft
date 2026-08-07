#pragma once
#include "Tick.hpp"
#include "world/level/TilePos.hpp"

struct TickNextTickData
{
	TickNextTickData(const TilePos& pos, TileID tileId, Tick_t tick)
		: pos(pos), tileId(tileId), tick(tick)
	{
	}

	TickNextTickData()
		: pos(), tileId(0), tick(0)
	{
	}

	TilePos pos;
	TileID tileId;
	Tick_t tick;
};

inline bool operator<(const TickNextTickData& data1, const TickNextTickData& data2)
{
	return data1.tick < data2.tick;
}

inline bool operator>(const TickNextTickData& data1, const TickNextTickData& data2)
{
	return data1.tick > data2.tick;
}

inline bool operator==(const TickNextTickData& data1, const TickNextTickData& data2)
{
	return data1.pos == data2.pos && data1.tileId == data2.tileId;
}
