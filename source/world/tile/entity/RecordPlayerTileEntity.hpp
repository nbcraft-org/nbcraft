#pragma once

#include "TileEntity.hpp"

class RecordPlayerTileEntity : public TileEntity
{
public:
    RecordPlayerTileEntity();

    void load(const CompoundTag& tag) override;
    void save(CompoundTag& tag) const override;

public:
    int m_record;
};
