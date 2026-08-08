#pragma once

#include "TileEntity.hpp"
#include "../Tile.hpp"

class PistonMovingTileEntity : public TileEntity
{
private:
    void _moveCollidedEntities(float progress, float force);

public:
    PistonMovingTileEntity(TileID tile = 0, int meta = 0, Facing::Name direction = Facing::DOWN, bool extending = false, bool isSourcePiston = false);

    int getTileId() const;
    //Not sure if they meant to override TileEntity::getData, as this should be the data from the tile at this pos
    int getData() const override;
    bool isExtending() const;
    Facing::Name getDirection() const;
    bool isSourcePiston() const;
    float getProgress(float) const;
    Vec3 getOff(float) const;

    void tick() override;

    void load(const CompoundTag& tag) override;
    void save(CompoundTag& tag) const override;

    void finalTick();

private:
    TileID m_tileId;
    int m_meta;
    Facing::Name m_direction;
    bool m_bExtending;
    bool m_bIsSourcePiston;
    float m_progress;
    float m_progressO;
};
