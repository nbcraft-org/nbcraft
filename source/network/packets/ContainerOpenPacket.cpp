#include "ContainerOpenPacket.hpp"
#include "network/NetEventCallback.hpp"

void ContainerOpenPacket::handle(const RakNet::RakNetGUID& guid, NetEventCallback& callback)
{
	callback.handle(guid, this);
}

void ContainerOpenPacket::write(RakNet::BitStream& bs)
{
	bs.Write((unsigned char)PACKET_CONTAINER_OPEN);
	bs.Write(m_containerId);
	bs.Write(m_type);
	bs.Write(m_size);
	bs.Write(m_title);
}

void ContainerOpenPacket::read(RakNet::BitStream& bs)
{
	bs.Read(m_containerId);
	bs.Read(m_type);
	bs.Read(m_size);
	bs.Read(m_title);
}
