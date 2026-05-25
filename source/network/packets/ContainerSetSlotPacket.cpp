#include "ContainerSetSlotPacket.hpp"
#include "network/NetEventCallback.hpp"
#include "network/PacketUtil.hpp"

ContainerSetSlotPacket::ContainerSetSlotPacket(int8_t containerId, int16_t slotId, const ItemStack& item)
	: m_containerId(containerId)
	, m_slotId(slotId)
	, m_item(item)
{
	_init();
}

void ContainerSetSlotPacket::handle(const RakNet::RakNetGUID& guid, NetEventCallback& callback)
{
	callback.handle(guid, this);
}

void ContainerSetSlotPacket::write(RakNet::BitStream& bs)
{
	bs.Write((unsigned char)PACKET_CONTAINER_SET_SLOT);
	bs.Write(m_containerId);
	bs.Write(m_slotId);
	PacketUtil::WriteItemStack(m_item, bs, false);
}

void ContainerSetSlotPacket::read(RakNet::BitStream& bs)
{
	bs.Read(m_containerId);
	bs.Read(m_slotId);
	m_item = PacketUtil::ReadItemStack(bs, false);
}
