#pragma once

#include "EntityTile.hpp"

class ChestTile : public EntityTile
{
public:
    ChestTile(int id, int texture);

public:
    int getTexture(TileSource& source, const TilePos& pos, Facing::Name face) const override;
    int getTexture(Facing::Name face) const override;
    bool mayPlace(TileSource& source, const TilePos& pos) const override;
    bool hasNeighbors(TileSource& source, const TilePos& pos, int count) const;
    void onRemove(TileSource& source, const TilePos& pos) override;
    bool use(const TilePos& pos, Player& player) override;
    bool hasTileEntity() const override;
    TileEntity* newTileEntity() override;

public:
    Random m_chestRandom;
};