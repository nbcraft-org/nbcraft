#pragma once

#include "../Packet.hpp"
#include "world/phys/Vec2.hpp"
#include "world/entity/SynchedEntityData.hpp"

class SetEntityDataPacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		m_channel = CHANNEL_ENTITY_EVENTS;
	}

public:
	SetEntityDataPacket()
		: m_entityId(0)
		, m_bIsIncoming(false)
	{
		_init();
	}
	SetEntityDataPacket(int32_t id, SynchedEntityData& data)
		: m_entityId(id)
		, m_bIsIncoming(false)
		, m_packedItems(data.packDirty())
	{
		_init();
	}

public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;
	const SynchedEntityData::ItemsArray& getUnpackedData() const { return m_packedItems; }

public:
	int32_t m_entityId;
	bool m_bIsIncoming;

private:
	SynchedEntityData::ItemsArray m_packedItems;
};
