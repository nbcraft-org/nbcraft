#pragma once

#include "../Packet.hpp"

class SetHealthPacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		m_channel = CHANNEL_PLAYER_EVENTS;
	}

public:
	SetHealthPacket(int8_t health = 0)
		: m_health(health)
	{
		_init();
	}

public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;

public:
	int8_t m_health; // @PARITY-JAVA: int16_t in Java
};
