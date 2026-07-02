#pragma once

#include "Tile.hpp"

class EntityTile : public Tile
{
public:
    EntityTile(TileID id, int textureID, Material* material);

public:
    virtual TileEntity* newTileEntity() = 0;

public:
	bool hasTileEntity() const override;
    void onPlace(TileSource& source, const TilePos& pos) override;
	void onRemove(TileSource& source, const TilePos& pos) override;
};