//
// Created by dooli on 2026/03/23.
//

#include "TileEntity.h"

#include "world/inventory/FurnaceTileEntity.h"
#include "world/level/Level.hpp"

TileEntity* TileEntity::createId(const std::string& id, const TilePos& pos) {
    if (id == "Furnace") return new FurnaceTileEntity(pos);
    // Easy to add more later! (e.g. ChestTileEntity when implemented)
    return NULL;
}

void TileEntity::markDirty() {
    if (level) {
        LevelChunk* chunk = level->getChunkAt(pos);
        if (chunk) chunk->markUnsaved();
    }
}
