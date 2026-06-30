#pragma once

#include <string>
#include "../Packet.hpp"

class MessagePacket : public Packet
{
private:
	void _init()
	{
		m_reliability = RELIABLE_ORDERED;
		m_channel = CHANNEL_CHAT;
	}

public:
	MessagePacket() { _init(); }
	MessagePacket(const std::string& msg)
	{
		_init();
		m_str = msg.c_str();
	}

	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;
public:
	RakNet::RakString m_str;
};
