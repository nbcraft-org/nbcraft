#pragma once

#include "../Packet.hpp"
#include "MoveEntityPacket.hpp"

class MoveEntityPacket_PosRot : public MoveEntityPacket
{
private:
	void _init();
public:
	MoveEntityPacket_PosRot();
	MoveEntityPacket_PosRot(int32_t entityId, const Vec3& pos, const Rot2& rot);
public:
	void handle(const RakNet::RakNetGUID&, NetEventCallback& callback) override;
	void write(RakNet::BitStream&) override;
	void read(RakNet::BitStream&) override;
};
