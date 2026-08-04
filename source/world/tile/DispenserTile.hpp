#pragma once

#include "EntityTile.hpp"

class DispenserTile : public EntityTile
{
public:
    DispenserTile(int id);

public:
    int getTickDelay() const override { return 4; };
    int getResource(TileData data, Random* random) const override { return Tile::dispenser->m_ID; };
    void onPlace(TileSource& source, const TilePos& pos) override;
    int getTexture(TileSource&, const TilePos& pos, Facing::Name face) const override;
    int getTexture(Facing::Name face) const override;
    bool use(const TilePos& pos, Player& player) override;
    void neighborChanged(TileSource&, const TilePos& pos, TileID tile) override;
    void tick(TileSource&, const TilePos& pos, Random*) override;
    TileEntity* newTileEntity() override;
    void setPlacedBy(const TilePos& pos, Mob&) override;
    bool hasTileEntity() const override;

private:
    void recalcLockDir(TileSource& source, const TilePos& pos);
    void fireArrow(TileSource& source, const TilePos& pos, Random* random);
};
