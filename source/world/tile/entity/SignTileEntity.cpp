#include "SignTileEntity.hpp"
#include "network/Packet.hpp"

SignTileEntity::SignTileEntity() 
    : m_selectedLine(-1)
{
    m_pType = TileEntityType::sign;
}

void SignTileEntity::load(const CompoundTag& tag)
{
    TileEntity::load(tag);
    m_messages[0] = tag.getString("Text1");
    m_messages[1] = tag.getString("Text2");
    m_messages[2] = tag.getString("Text3");
    m_messages[3] = tag.getString("Text4");
}

void SignTileEntity::save(CompoundTag& tag) const
{
    TileEntity::save(tag);
    tag.putString("Text1", m_messages[0]);
    tag.putString("Text2", m_messages[1]);
    tag.putString("Text3", m_messages[2]);
    tag.putString("Text4", m_messages[3]);
}

//Packet* SignTileEntity::getUpdatePacket()
//{
//    return new SignUpdatePacket(m_pos, m_messages);
//}
