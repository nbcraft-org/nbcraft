#pragma once

#include "TileEntity.hpp"
#include "world/level/Level.hpp"
#include "world/inventory/SimpleContainer.hpp"

class DispenserTileEntity : public TileEntity, public SimpleContainer
{
public:
    DispenserTileEntity();

public:
    ItemStack removeRandomItem();
    void load(const CompoundTag& tag) override;
    void save(CompoundTag& tag) const override;

    bool stillValid(Player* player) const override;

    void setContainerChanged(StackID stackId) override;

private:
    Random m_random;
};
