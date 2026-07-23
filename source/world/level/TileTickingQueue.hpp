#pragma once
#include "world/tile/Tile.hpp"
#include "Tick.hpp"
#include "TickNextTickData.hpp"
#include "common/MovePriorityQueue.hpp"

class TileSource;

class TileTickingQueue
{
private:
	Tick_t m_currentTick;
	MovePriorityQueue<TickNextTickData, std::greater<TickNextTickData> > m_tickData;
	bool m_instaTick;
	Random m_random;

public:
	TileTickingQueue();

private:
	void _tick(TileSource& region, const TilePos& pos, TileID tileID);

public:
	void add(TileSource& region, const TilePos& pos, TileID tileID, int tickDelay);

	bool tickPendingTicks(TileSource& region, Tick_t until, int max, bool instaTick);
	void tickAllPendingTicks(TileSource& region);

	void save(CompoundTag& tag);
	void load(const CompoundTag& tag);
};
