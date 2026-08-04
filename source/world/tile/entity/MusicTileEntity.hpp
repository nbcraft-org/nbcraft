#pragma once

#include "TileEntity.hpp"

class MusicTileEntity : public TileEntity
{
public:
    MusicTileEntity();

public:
    void load(const CompoundTag& tag) override;
    void save(CompoundTag& tag) const override;

public:
    void tune();
    void play(TileSource& source, const TilePos& pos);

public:
    uint8_t m_note;
    bool m_bOn;
};
