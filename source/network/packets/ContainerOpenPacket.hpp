#pragma once

#include <string>
#include "../Packet.hpp"
#include "world/inventory/Container.hpp"

class ContainerOpenPacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		m_channel = CHANNEL_CONTAINERS;
	}

public:
	ContainerOpenPacket()
		: m_containerId(0)
		, m_type(Container::CONTAINER)
		, m_size(0)
	{
		_init();
	}
	ContainerOpenPacket(int8_t containerId, Container::Type type, const std::string& title, int8_t size)
		: m_containerId(containerId)
		, m_type(type)
		, m_title(title.c_str())
		, m_size(size)
	{
		_init();
	}

public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;

public:
	int8_t m_containerId;
	int8_t m_type;
	RakNet::RakString m_title;
	int8_t m_size;
};
