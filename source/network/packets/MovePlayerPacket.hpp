#pragma once

#include "../Packet.hpp"
#include "world/phys/Vec2.hpp"
#include "world/phys/Vec3.hpp"

class MovePlayerPacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		// we could probably get away with doing this on a different channel, but we'd want all player events
		// to be ordered when using server-authoritative movement in order to avoid rollbacks
		m_channel = CHANNEL_PLAYER_EVENTS;
	}

public:
	MovePlayerPacket()
		: m_id(0)
	{
		_init();
	}
	MovePlayerPacket(int id, const Vec3& pos, const Vec2& rot)
		: m_id(id)
		, m_pos(pos)
		, m_rot(rot)
	{
		_init();
	}

public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;

public:
	int m_id;
	Vec3 m_pos;
	Vec2 m_rot;
};
