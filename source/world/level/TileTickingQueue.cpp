#include "TileTickingQueue.hpp"
#include "world/level/TileSource.hpp"
#include "nbt/CompoundTag.hpp"
#include "nbt/ListTag.hpp"

TileTickingQueue::TileTickingQueue()
{
	m_currentTick = 0;
	m_instaTick = false;
}

void TileTickingQueue::_tick(TileSource& region, const TilePos& pos, TileID tileId)
{
	if (tileId == 0)
		return;

	TileID expectedTileId = region.getTile(pos);
	if (tileId == expectedTileId)
		Tile::tiles[tileId]->tick(region, pos, &m_random);
}

void TileTickingQueue::add(TileSource& region, const TilePos& pos, TileID tileId, int tickDelay)
{
	if (!region.hasChunksAt(pos, 8))
		return;

	if (tickDelay < 0)
	{
		_tick(region, pos, tileId);
	}
	else
	{
		m_tickData.push(TickNextTickData(pos, tileId, m_currentTick + tickDelay));
	}
}

bool TileTickingQueue::tickPendingTicks(TileSource& region, Tick_t until, int max, bool instaTick)
{
	m_instaTick = instaTick;

	int tickLimit = Mth::Min(m_tickData.size(), max);
	int ticksProcessed = 0;

	bool hasTicked = false;
	while (!m_tickData.empty() && m_tickData.top().tick <= until && ticksProcessed < tickLimit)
	{
		TickNextTickData data;
		m_tickData.popInto(data);

		m_currentTick = data.tick;
		ticksProcessed++;

		if (region.hasChunksAt(data.pos - 8, data.pos + 8))
			_tick(region, data.pos, data.tileId);

		hasTicked = true;
	}

	m_currentTick = until;
	m_instaTick = false;

	return hasTicked;
}

// copy-paste of the function above, done for Level, remove in the future
bool TileTickingQueue::tickPendingTicks(TileSource& region, int max, bool instaTick)
{
	m_instaTick = instaTick;

	int tickLimit = Mth::Min(m_tickData.size(), max);
	int ticksProcessed = 0;

	bool hasTicked = false;
	while (!m_tickData.empty() && ticksProcessed < tickLimit)
	{
		TickNextTickData data;
		m_tickData.popInto(data);

		m_currentTick = data.tick;
		ticksProcessed++;

		if (region.hasChunksAt(data.pos - 8, data.pos + 8))
			_tick(region, data.pos, data.tileId);

		hasTicked = true;
	}

	m_instaTick = false;

	return hasTicked;
}

void TileTickingQueue::tickAllPendingTicks(TileSource& region)
{
	while (!m_tickData.empty())
	{
		tickPendingTicks(region, Tick::MAX, INT32_MAX, true);
	}
}

void TileTickingQueue::save(CompoundTag& tag)
{
	ListTag* list = new ListTag();

	for (MovePriorityQueue<TickNextTickData>::Iterator iter = m_tickData.begin(); iter != m_tickData.end(); iter++)
	{
		const TickNextTickData& data = *iter;

		CompoundTag* childTag = new CompoundTag();
		childTag->putInt32("x", data.pos.x);
		childTag->putInt32("y", data.pos.y);
		childTag->putInt32("z", data.pos.z);
		childTag->putInt8("tileID", static_cast<int8_t>(data.tileId));
		childTag->putInt64("time", static_cast<int64_t>(data.tick));
		list->add(childTag);
	}

	tag.put("tickList", list);
}

void TileTickingQueue::load(const CompoundTag& tag)
{
	const ListTag* list = tag.getList("tickList");

	for (size_t i = 0; i < list->size(); i++)
	{
		const CompoundTag* childTag = list->getCompound(i);

		TilePos pos = TilePos(childTag->getInt32("x"), childTag->getInt32("y"), childTag->getInt32("z"));
		TileID tileID = static_cast<TileID>(childTag->getInt8("tileID"));
		Tick_t tick = static_cast<Tick_t>(childTag->getInt64("tick"));
		m_tickData.push(TickNextTickData(pos, tileID, tick));
	}
}
