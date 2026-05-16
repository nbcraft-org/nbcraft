#include "SendInventoryPacket.hpp"
#include "network/NetEventCallback.hpp"
#include "network/PacketUtil.hpp"

SendInventoryPacket::SendInventoryPacket(int32_t entityId, bool dropAll)
	: m_entityId(entityId)
	, m_count(0)
	, m_extra(dropAll ? EXTRA_DROP_ALL : EXTRA_NONE)
{
}

void SendInventoryPacket::handle(const RakNet::RakNetGUID& guid, NetEventCallback& callback)
{
	callback.handle(guid, this);
}

void SendInventoryPacket::write(RakNet::BitStream& bs)
{
	bs.Write((unsigned char)PACKET_SEND_INVENTORY);
	bs.Write(m_entityId);
	bs.Write(m_extra);
	bs.Write((uint16_t)m_items.size());

	for (size_t i = 0; i < m_items.size(); i++)
	{
		PacketUtil::WriteItemStack(m_items[i], bs, false);
	}
}

void SendInventoryPacket::read(RakNet::BitStream& bs)
{
	bs.Read(m_entityId);
	bs.Read(m_extra);
	bs.Read(m_count);

	m_count = Mth::Min(m_count, 512);
	m_items.reserve(m_count);

	for (uint16_t i = 0; i < m_count; i++)
	{
		m_items.push_back(PacketUtil::ReadItemStack(bs, false));
	}
}
