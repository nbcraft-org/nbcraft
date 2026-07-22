/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <stdint.h>
#include "common/Utils.hpp"
#include "world/level/Tick.hpp"
#include "world/level/TilePos.hpp"

struct TickNextTickData
{
public:
	TilePos pos;
	TileID tileId;
	Tick_t tick;

public:
	TickNextTickData()
		: pos(), tileId(0), tick(0)
	{
	}
	TickNextTickData(const TilePos& pos, TileID tileId, Tick_t tick)
		: pos(pos), tileId(tileId), tick(tick)
	{
	}

public:
	bool operator<(const TickNextTickData& other) const
	{
		return tick < other.tick;
	}
	bool operator>(const TickNextTickData& other) const
	{
		return tick > other.tick;
	}
	bool operator==(const TickNextTickData& other) const
	{
		return pos == other.pos && tileId == other.tileId;
	}
};

/*struct TickNextTickData
{
	static int C;

	TickNextTickData(const TilePos& tilePos, TileID tileId);
	int hashCode() const;
	bool operator<(const TickNextTickData& other) const;
	bool operator==(const TickNextTickData& other) const;
	void setDelay(int32_t delay) { this->delay = delay; }

	int id;
	TilePos tilePos;
	TileID tileId;
	int32_t delay;
};*/
