#pragma once

#include "EntityTile.hpp"

class DispenserTile : public EntityTile
{
public:
    DispenserTile(int id);

public:
    int getTickDelay() const override { return 4; };
    int getResource(TileData data, Random* random) const override { return Tile::dispenser->m_ID; };
    void onPlace(Level* level, const TilePos& pos) override;
    int getTexture(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
    int getTexture(Facing::Name face) const override;
    bool use(Level* level, const TilePos& pos, Player* var5) override;
    void neighborChanged(Level*, const TilePos& pos, TileID tile) override;
    void tick(Level*, const TilePos& pos, Random*) override;
    TileEntity* newTileEntity() override;
    void setPlacedBy(Level*, const TilePos& pos, Mob*) override;
    bool hasTileEntity() const override;

private:
    void recalcLockDir(Level* level, const TilePos& pos);
    void fireArrow(Level* level, const TilePos& pos, Random* random);
};
