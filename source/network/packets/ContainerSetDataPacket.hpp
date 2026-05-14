#pragma once

#include "../Packet.hpp"

class ContainerSetDataPacket : public Packet
{
public:
	ContainerSetDataPacket()
	{
		m_containerId = 0;
		m_id = 0;
		m_value = 0;
	}
	ContainerSetDataPacket(int8_t containerId, int16_t id, int16_t value);

	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;
public:
	int8_t m_containerId;
	int16_t m_id;
	int16_t m_value;
};