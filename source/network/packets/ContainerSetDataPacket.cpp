#include "ContainerSetDataPacket.hpp"
#include "network/NetEventCallback.hpp"

ContainerSetDataPacket::ContainerSetDataPacket(int8_t containerId, int16_t slotId, int16_t value)
	: m_containerId(containerId)
	, m_slotId(slotId)
	, m_value(value)
{
}

void ContainerSetDataPacket::handle(const RakNet::RakNetGUID& guid, NetEventCallback& callback)
{
	callback.handle(guid, this);
}

void ContainerSetDataPacket::write(RakNet::BitStream& bs)
{
	bs.Write((unsigned char)PACKET_CONTAINER_SET_DATA);
	bs.Write(m_containerId);
	bs.Write(m_slotId);
	bs.Write(m_value);
}

void ContainerSetDataPacket::read(RakNet::BitStream& bs)
{
	bs.Read(m_containerId);
	bs.Read(m_slotId);
	bs.Read(m_value);
}
