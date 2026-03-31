//
// Created by Dominic Hann on 23/3/2026.
//

#ifndef NBCRAFT_FURNACETILE_HPP
#define NBCRAFT_FURNACETILE_HPP
#include "Tile.hpp"


class FurnaceTile : public Tile {
public:
    FurnaceTile(TileID id);

public:
    bool use(Level*, const TilePos& pos, Player*) override;
    int getResource(TileData, Random*) const override;
    int getTexture(Facing::Name face) const override;
    int getTexture(Facing::Name face, TileData data) const override;
    virtual void onPlace(Level* level, const TilePos& pos) override;
    virtual void onRemove(Level* level, const TilePos& pos) override;
    virtual void setPlacedBy(Level* level, const TilePos& pos, Mob* mob) override;

    virtual TileEntity* createTileEntity(const TilePos& pos);

    // Set to true before swapping between TILE_FURNACE and TILE_FURNACE_LIT
    // to prevent onRemove/onPlace from deleting/creating the tile entity.
    static bool s_swappingLitState;
};



#endif //NBCRAFT_FURNACETILE_HPP