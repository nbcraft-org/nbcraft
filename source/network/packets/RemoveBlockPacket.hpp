#pragma once

#include "../Packet.hpp"
#include "world/level/TilePos.hpp"

class RemoveBlockPacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		m_channel = CHANNEL_TILE_EVENTS;
	}

public:
	RemoveBlockPacket()
		: m_entityId(0)
	{
		_init();
	}
	RemoveBlockPacket(int id, const TilePos& pos)
		: m_entityId(id), m_pos(pos)
	{
		_init();
	}

public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;

public:
	int m_entityId;
	TilePos m_pos;
};
