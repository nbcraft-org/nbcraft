#pragma once

#include "../Packet.hpp"
#include "world/item/ItemStack.hpp"

class ContainerSetSlotPacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		m_channel = CHANNEL_CONTAINERS;
	}

public:
	ContainerSetSlotPacket()
		: m_containerId(0)
		, m_slotId(0)
	{
		_init();
	}
	ContainerSetSlotPacket(int8_t containerId, int16_t slotId, const ItemStack& item);

public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;

public:
	int8_t m_containerId;
	int16_t m_slotId;
	ItemStack m_item;
};
