#pragma once

#include "../Packet.hpp"

class PlayerEquipmentPacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		m_channel = CHANNEL_PLAYER_EVENTS;
	}

public:
	PlayerEquipmentPacket()
		: m_playerID(0)
		, m_itemID(0)
		, m_itemAuxValue(0)
	{
		_init();
	}
	PlayerEquipmentPacket(int playerID, int itemID, uint16_t itemAuxValue)
		: m_playerID(playerID)
		, m_itemID(itemID < 0 ? 0 : itemID)
		, m_itemAuxValue(itemAuxValue)
	{
		_init();
	}

public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;

public:
	int m_playerID;
	uint16_t m_itemID;
	uint16_t m_itemAuxValue;
};
