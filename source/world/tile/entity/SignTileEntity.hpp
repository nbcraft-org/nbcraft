#pragma once

#include <string>
#include "TileEntity.hpp"

class SignTileEntity : public TileEntity
{
public:
    SignTileEntity();

    void load(const CompoundTag& tag) override;
    void save(CompoundTag& tag) const override;
    //Packet* getUpdatePacket() override;

public:
    std::string m_messages[4];
    int m_selectedLine;
};
