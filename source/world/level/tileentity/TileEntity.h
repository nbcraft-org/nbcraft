#ifndef TILEENTITY_H
#define TILEENTITY_H

#include <string>
#include "world/level/TilePos.hpp"
#include "nbt/CompoundTag.hpp"

class Level;

class TileEntity {
public:
    // 1. Declaration order: pos, then level, then id
    TilePos pos;
    Level* level;
    std::string id;

    // 2. Constructor: Must match the order above!
    TileEntity(const std::string& id, const TilePos& pos)
        : pos(pos), level(NULL), id(id) {}

    // 3. VIRTUAL destructor is required for OOP!
    virtual ~TileEntity() {}

    virtual void load(const CompoundTag& tag) {
        // If 'getInt' fails, check CompoundTag.hpp for 'getInt32' or 'put'
        pos.x = tag.getInt32("x");
        pos.y = tag.getInt32("y");
        pos.z = tag.getInt32("z");
    }

    virtual void save(CompoundTag& tag) const {
        tag.putString("id", id);
        tag.putInt32("x", pos.x);
        tag.putInt32("y", pos.y);
        tag.putInt32("z", pos.z);
    }

    virtual void tick() {}

    // Marks the owning chunk as needing to be saved.
    void markDirty();

    static TileEntity* createId(const std::string& id, const TilePos& pos);
};

#endif