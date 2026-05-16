#pragma once

#include <vector>

#include "../Packet.hpp"
#include "world/item/ItemStack.hpp"

class SendInventoryPacket : public Packet
{
public:
	enum Extra
	{
		EXTRA_NONE,
		EXTRA_DROP_ALL
	};

public:
	SendInventoryPacket()
	{
		m_entityId = 0;
		m_count = 0;
		m_extra = EXTRA_NONE;
	}
	SendInventoryPacket(int32_t entityId, bool dropAll = false);

	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;
public:
	int32_t m_entityId;
	std::vector<ItemStack> m_items;
	uint16_t m_count;
	uint8_t m_extra;
};
