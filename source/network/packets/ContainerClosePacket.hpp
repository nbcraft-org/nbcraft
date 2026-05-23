#pragma once

#include "../Packet.hpp"

class ContainerClosePacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		m_channel = CHANNEL_CONTAINERS;
	}

public:
	ContainerClosePacket()
		: m_containerId(0)
	{
		_init();
	}
	ContainerClosePacket(int8_t containerId)
		: m_containerId(containerId)
	{
		_init();
	}
	
public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;

public:
	int8_t m_containerId;
};
